#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */

// Appends a node containing ucontext_t i to the end of the queue
// Only used for part 2
void listappend(node **head, ucontext_t *i) {
	node *n = malloc(sizeof(node));
	n -> ctx = *i;

	if(head==NULL) {
		head = &n;
		return;
	}

	node *temp = *head;
	while(temp !=NULL) {
		temp = temp->next;
	}
	temp = n;
}

// Removes and returns the ucontext_t value in the top node of the queue
// Should never be called if list is empty
ucontext_t *listremove(node **head) {
	node *temp = *head;
	head = &(temp -> next);
	return &(temp->ctx);
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
	
	(**tail).next = *head;
	*tail = *head;
	*head = (**head).next;
	(**tail).next = NULL;
		
	return;
}














