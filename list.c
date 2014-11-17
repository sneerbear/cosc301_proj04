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

void addctx(node** head, node** tail, ucontext_t* returnctx){
	
	node* thread = malloc(sizeof(node));
	thread->next = NULL;
	
	if(*head == NULL){
		*head = thread;
		*tail = thread;
	}
	
	else{
		(**tail).next = thread;
		*tail = thread;
	}
	
	#define STACKSIZE 128000
	unsigned char *stack = (unsigned char *)malloc(STACKSIZE);
	assert(stack);

	getcontext(&thread->ctx);
	thread->ctx.uc_stack.ss_sp   = stack;
	thread->ctx.uc_stack.ss_size = STACKSIZE;
	thread->ctx.uc_link          = returnctx;
	
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

void headdestroy(node **head){
	
	node* tmp = *head;
	*head = (**head).next;
	free(tmp->ctx.uc_stack.ss_sp);
	free(tmp);
	
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
	
	(**tail).next = *head;
	*tail = *head;
	*head = (**head).next;
	(**tail).next = NULL;
	
	return;
}















