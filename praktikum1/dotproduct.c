# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>

int main ( int argc, char *argv[] );
double test01 ( int n, double x[], double y[] );
double test02 ( int n, double x[], double y[] );
void do_computation_time_measurement();

/******************************************************************************/

int main ( int argc, char *argv[] )
{
  double factor;
  int i;
  int n;
  double wtime;
  double *x;
  double xdoty;
  double *y;
  double launch_time;
  double done_time;

  n = 100;

  while ( n < 1000000 )
  {
    n = n * 10;

    x = ( double * ) malloc ( n * sizeof ( double ) );
    y = ( double * ) malloc ( n * sizeof ( double ) );

    factor = ( double ) ( n );
    factor = 1.0 / sqrt ( 2.0 * factor * factor + 3 * factor + 1.0 );

    for ( i = 0; i < n; i++ )
    {
      x[i] = ( i + 1 ) * factor;
    }

    for ( i = 0; i < n; i++ )
    {
      y[i] = ( i + 1 ) * 6 * factor;
    }

    printf ( "\n" );
/*
  Test #1
*/
    launch_time = omp_get_wtime();
    xdoty = test01(n,x,y);
    done_time = omp_get_wtime();
    wtime = done_time - launch_time;
    printf ( "  Sequential  %8d  %14.6e  %15.10f\n", n, xdoty, wtime );
/*
  Test #2
*/
    launch_time = omp_get_wtime();
    xdoty = test02(n,x,y);
    done_time = omp_get_wtime();
    wtime = done_time - launch_time;
    printf ( "  Parallel    %8d  %14.6e  %15.10f\n", n, xdoty, wtime );

    free ( x );
    free ( y );
  }
/*
  Terminate.
*/
  printf ( "\n" );
  printf ( "DOT_PRODUCT\n" );
  printf ( "  Normal end of execution.\n" );

  do_computation_time_measurement();

  return 0;
}

//Sequential version
double test01 ( int n, double x[], double y[] )

{
  int i;
  double xdoty;
  xdoty = 0.0;

  for (i=0; i<n; i++) {
    xdoty = xdoty + x[i]*y[i];
  }

  return xdoty;
}

//Parallel version
double test02 ( int n, double x[], double y[] )

{
  int i;
  double xdoty;
  xdoty = 0.0;

  # pragma omp parallel for reduction(+:xdoty)
  for (i=0; i<n; i++) {
    xdoty = xdoty + x[i]*y[i];
  }

  return xdoty;
}

// Exercise 3c
void do_computation_time_measurement() {
  int n;
  int numThreads;
  double launch_time;
  double done_time;
  double wtime;
  double factor;
  double *x;
  double *y;
  int i;
  double xdoty;

  // Building vectors
  n = 10000000;

  x = ( double * ) malloc ( n * sizeof ( double ) );
  y = ( double * ) malloc ( n * sizeof ( double ) );

  factor = ( double ) ( n );
  factor = 1.0 / sqrt ( 2.0 * factor * factor + 3 * factor + 1.0 );

  for ( i = 0; i < n; i++ )
  {
    x[i] = ( i + 1 ) * factor;
    y[i] = ( i + 1 ) * 6 * factor;
  }

  // Starting ...
  printf("\n");
  printf("COMPUTATION TIME MEASUREMENT:\n");

  numThreads = 1;

  // Sequential
  launch_time = omp_get_wtime();
  xdoty = test01(n,x,y);
  done_time = omp_get_wtime();
  wtime = done_time - launch_time;
  printf("  Sequential:             %15.10f;  xdoty = %14.6e\n",wtime,xdoty);

  while (numThreads <= 16) {
    omp_set_num_threads(numThreads);
    // Parallel
    launch_time = omp_get_wtime();
    xdoty = test02(n,x,y);
    done_time = omp_get_wtime();
    wtime = done_time - launch_time;

    printf("  Parallel (%d Threads):   %15.10f;   xdoty = %14.6e\n",numThreads,wtime,xdoty);

    numThreads = 2 * numThreads;
  }
  printf("ENDING MEASUREMENTS...\n");
}
