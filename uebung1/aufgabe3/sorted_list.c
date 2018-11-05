#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "sorted_list.h"

struct SortedLinkedListNode
{
	SortedLinkedListNode* next;
	int data;
};

struct SortedLinkedList
{
	SortedLinkedListNode* first;
};

SortedLinkedList* SortedLinkedList_create() {
	SortedLinkedList* linkedList = malloc( sizeof(SortedLinkedList) );
	linkedList->first = NULL;
	return linkedList;
}

void SortedLinkedList_addToList( SortedLinkedList* list, int data ) {
	SortedLinkedListNode* new_node = malloc( sizeof(SortedLinkedListNode) );
	new_node->data = data;
	new_node->next = NULL;
	if (list->first == NULL) {
		list->first = new_node;
		return;
	}
	SortedLinkedListNode* actual_node = list->first;
	if (actual_node->data > data) {
		new_node->next = actual_node;
		list->first = new_node;
		return;
	}
	while (actual_node != NULL) {
		if (actual_node->data > data) {
			new_node->next = actual_node;
			break;
		}
		actual_node = actual_node->next;
	}
	actual_node = new_node;
	return;
}

void SortedLinkedList_delete( SortedLinkedList* list ) {
	if (list->first == NULL) free( list ); 
	else {
		SortedLinkedListNode* actual_node = list->first;
		SortedLinkedListNode* next_node;
		while (actual_node != NULL) {
			next_node = actual_node->next;
			free( actual_node );
			actual_node = next_node;
		}
	}
	free( list );
	return;
}

SortedLinkedListNode* SortedLinkedList_getSmallest( SortedLinkedList* list ) {
	if (list->first == NULL) return NULL;
	else return list->first;
}

int main() {
	printf("Starting test procedure...\n");
	printf("Creating SortedLinkedList...\n");
	SortedLinkedList* list = SortedLinkedList_create();
	printf("Adress of list is %p.\n",list);
	printf("Adding node with data = 3 into list.\n");
	SortedLinkedList_addToList(list,3);
	printf("Getting smallest value...\n");
	SortedLinkedListNode* smallestNode = SortedLinkedList_getSmallest(list);
	printf("Smallest value is %d.\n",smallestNode->data);
	printf("Adding node with data = 5 into list.\n");
	SortedLinkedList_addToList(list,5);
	printf("Getting smallest value...\n");
	SortedLinkedListNode* shouldBeSameAsSmallestNode = SortedLinkedList_getSmallest(list);
	printf("Smallest value is %d.\n",shouldBeSameAsSmallestNode->data);
	printf("Adding node with data = 1 into list.\n");
	SortedLinkedList_addToList(list,1);
	printf("Getting smallest value...\n");
	SortedLinkedListNode* newSmallestNode = SortedLinkedList_getSmallest(list);
	printf("Smallest value is %d.\n",newSmallestNode->data);
	printf("Deleting list...\n");
	SortedLinkedList_delete(list);
}