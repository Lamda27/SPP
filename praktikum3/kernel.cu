//
//  kernel.cu
//
//  Created by Arya Mazaheri on 01/12/2018.
//

#include <iostream>
#include <algorithm>
#include <cmath>
#include "ppm.h"

using namespace std;

/*********** Gray Scale Filter  *********/

/**
 * Converts a given 24bpp image into 8bpp grayscale using the GPU.
 */
__global__
void cuda_grayscale(int width, int height, BYTE *image, BYTE *image_out)
{
	//int thread = threadIdx.x + threadIdx.y * blockDim.x + blockIdx.x * blockDim.x * blockDim.y;

	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int thread = x + gridDim.x * blockDim.x * y;

	if(thread >= width*height)
		return;

	int pixel_out = thread;
	int pixel_in = 3*thread;
	image_out[pixel_out] = image[pixel_in] * 0.0722f + image[pixel_in + 1] * 0.7152f + image[pixel_in + 2] * 0.2126f;

}


// 1D Gaussian kernel array values of a fixed size (make sure the number > filter size d)
//TODO: Define the cGaussian array on the constant memory (2 pt)

void cuda_updateGaussian(int r, double sd)
{
	float fGaussian[64];
	for (int i = 0; i < 2*r +1 ; i++)
	{
		float x = i - r;
		fGaussian[i] = expf(-(x*x) / (2 * sd*sd));
	}
	//TODO: Copy computed fGaussian to the cGaussian on device memory (2 pts)
	float cGaussian[64];
	cudaMemset(cGaussian, 0, 64);
	cudaMemcpyToSymbol(cGaussian, fGaussian, 64 * sizeof(float), 0, cudaMemcpyHostToDevice);
}

//TODO: implement cuda_gaussian() kernel (3 pts)
__device__
inline double cuda_gaussian(float x, double sigma){
	return expf(-(powf(x, 2)) / (2 * powf(sigma, 2)));
}

/*********** Bilateral Filter  *********/
// Parallel (GPU) Bilateral filter kernel
__global__ void cuda_bilateral_filter(BYTE* input, BYTE* output,
	int width, int height,
	int r, double sI, double sS)
{
	// 1D Gaussian kernel array values of a fixed size (make sure the number > filter size r
	float fGaussian[64];
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int thread = x + gridDim.x * blockDim.x * y;

	if(thread >= width*height)
		return;

	double iFiltered = 0;
	double wP = 0;
	unsigned char centrePx = input[thread];

	for (int dy = -r; dy <= r; dy++) {
		int neighborY = y+dy;
		if (neighborY < 0)
			neighborY = 0;
		else if (neighborY >= height)
			neighborY = height - 1;
		for (int dx = -r; dx <= r; dx++) {
			int neighborX = x+dx;
			if (neighborX < 0)
				neighborX = 0;
			else if (neighborX >= width)
				neighborX = width - 1;
			int currPx_idx = neighborX + gridDim.x * blockDim.x * neighborY;
			unsigned char currPx = input[currPx_idx];
			double w = (fGaussian[dy+r] * fGaussian[dx+r]) * cuda_gaussian(centrePx - currPx, sI);
			iFiltered += x * currPx;
			wP += x;
		}
	}
output[thread] = iFiltered / wP;
}


void gpu_pipeline(const Image & input, Image & output, int r, double sI, double sS)
{
	printf("Starting gpu_pipeline.\n");
	// Events to calculate gpu run time
	cudaEvent_t start, stop;
	float time;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	// GPU related variables
	BYTE *d_input = NULL;
	BYTE *d_image_out[2] = {0}; //temporary output buffers on gpu device
	int image_size = input.cols*input.rows;
	int suggested_blockSize;   // The launch configurator returned block size
	int suggested_minGridSize; // The minimum grid size needed to achieve the maximum occupancy for a full device launch

	// ******* Grayscale kernel launch *************
	printf("Launching grayscale kernel.\n");
	//Creating the block size for grayscaling kernel
	cudaOccupancyMaxPotentialBlockSize( &suggested_minGridSize, &suggested_blockSize, cuda_grayscale);

        int block_dim_x, block_dim_y;
        block_dim_x = (int) sqrt(suggested_blockSize);
				block_dim_y = (int) sqrt(suggested_blockSize);

        dim3 gray_block(block_dim_x, block_dim_y); // 2 pts

        //TODO: Calculate grid size to cover the whole image - 2 pts
				int grid_dim_x, grid_dim_y;
				grid_dim_x = (int) sqrt(suggested_minGridSize);
				grid_dim_y = (int) sqrt(suggested_minGridSize);
				dim3 gray_grid(grid_dim_x, grid_dim_y);

        // Allocate the intermediate image buffers for each step
        Image img_out(input.cols, input.rows, 1, "P5");
				printf("Test.\n");
        for (int i = 0; i < 2; i++)
        {
            //TODO: allocate memory on the device (2 pts)
            //TODO: intialize allocated memory on device to zero (2 pts)
							cudaMalloc(&d_image_out[i], image_size * sizeof(BYTE));
							cudaMemset(d_image_out[i], 0, image_size * sizeof(BYTE));
        }

        //copy input image to device
        //TODO: Allocate memory on device for input image (2 pts)
				cudaMalloc(&d_input, image_size * sizeof(BYTE));
				cudaMemset(d_input, 0, image_size * sizeof(BYTE));
        //TODO: Copy input image into the device memory (2 pts)
				cudaMemcpy(d_input, &input, image_size * sizeof(BYTE), cudaMemcpyHostToDevice);

        cudaEventRecord(start, 0); // start timer
        // Convert input image to grayscale
        //TODO: Launch cuda_grayscale() (2 pts)
				cudaDeviceSynchronize();
				cuda_grayscale<<<gray_grid, gray_block>>>(input.cols, input.rows, d_input, d_image_out[0]);

        cudaEventRecord(stop, 0); // stop timer
        cudaEventSynchronize(stop);

        // Calculate and print kernel run time
				cudaEventElapsedTime(&time, start, stop);
				cout << "GPU Grayscaling time: " << time << " (ms)\n";
				cout << "Launched blocks of size " << gray_block.x * gray_block.y << endl;

        //TODO: transfer image from device to the main memory for saving onto the disk (2 pts)
				cudaMemcpy(d_image_out[0], &img_out, image_size * sizeof(BYTE), cudaMemcpyDeviceToHost);

        savePPM(img_out, "image_gpu_gray.ppm");


	// ******* Bilateral filter kernel launch *************

	//Creating the block size for grayscaling kernel
	cudaOccupancyMaxPotentialBlockSize( &suggested_minGridSize, &suggested_blockSize, cuda_bilateral_filter);

        block_dim_x = block_dim_y = (int) sqrt(suggested_blockSize);

        dim3 bilateral_block(block_dim_x,block_dim_y); // 2 pts

        //TODO: Calculate grid size to cover the whole image - 2pts
				grid_dim_x = grid_dim_y = (int) sqrt(suggested_minGridSize);
				dim3 bilateral_grid(grid_dim_x, grid_dim_y);

        // Create gaussain 1d array
        cuda_updateGaussian(r,sS);

        cudaEventRecord(start, 0); // start timer
	//TODO: Launch cuda_bilateral_filter() (2 pts)
				cuda_bilateral_filter<<<bilateral_grid, bilateral_block>>>(d_image_out[0], d_image_out[1],
					input.cols, input.cols, r, sI, sS);

        cudaEventRecord(stop, 0); // stop timer
        cudaEventSynchronize(stop);

        // Calculate and print kernel run time
        cudaEventElapsedTime(&time, start, stop);
        cout << "GPU Bilateral Filter time: " << time << " (ms)\n";
        cout << "Launched blocks of size " << bilateral_block.x * bilateral_block.y << endl;

        // Copy output from device to host
	//TODO: transfer image from device to the main memory for saving onto the disk (2 pts)
				cudaMemcpy(d_image_out[1], &img_out, image_size * sizeof(BYTE), cudaMemcpyDeviceToHost);
				savePPM(img_out, "image_gpu_bilateral.ppm");

        // ************** Finalization, cleaning up ************
				printf("Device Error:\t%s\n", cudaGetErrorString(cudaGetLastError()));

        // Free GPU variables
	//TODO: Free device allocated memory (3 pts)
				cudaFree(d_input);
				cudaFree(d_image_out[0]);
				cudaFree(d_image_out[1]);
}
