#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */

void listadd(node **head, ucontext_t i) {
	
	//create new node for entry
	node *n = malloc(sizeof(node));
	n->ctx = i;
	//if the list is empty
	if(*head == NULL){
		n->next = *head;
		*head = n;
		return;
	}
	
	//initialize variables for running through the list
	node *runner = *head;
	node *previous = *head;
	runner = runner -> next;
	
	//loop through list until the end is reached
	while(runner != NULL) {
		previous = runner;
		runner = runner -> next;
	}
	
	//place new node in the proper location
	previous->next = n;
	n->next = runner;
}

void listdestroy(node *list) {
	// Iterate through list and free all nodes
    while (list != NULL) {
        node *tmp = list;
        list = list->next;
		unsigned char *stack = tmp->ctx.uc_stack.ss_sp;
		free(stack);
        free(tmp);
    }
}

void listprint(node *list) {
	printf("*** List Contents Begin ***\n");
	// Iterate through list and print all nodes
	int i = 0;
    while (list != NULL) {
		printf("Thread %d\n",i++);
        list = list->next;
    }
	printf("*** List Contents End ***\n");
}

void nextthread(node **head, node **tail){
	node *old = *head;
	
	while(1){
		(**tail).next = *head;
		*tail = *head;
		*head = (**head).next;
		(**tail).next = NULL;
		if(!(**head).finished || *head == old){
			if(*head == old){
				//printf("No Threads Left\n");
			}
			break;
		}
	}
	
	return;
}















