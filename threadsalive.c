#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ucontext.h>
#include "threadsalive.h"
#include "list.c"
#include "list.h"

/* ***************************** 
     stage 1 library functions
   ***************************** */

node *head;
node *tail;
ucontext_t mainctx;

void ta_libinit(void) {
	
	printf("Library \"initialized\"\n");
	
    return;
}

void ta_create(void (*func)(void *), void *arg) {
	
	node* thread = malloc(sizeof(node));
	thread->next = NULL;
	
	if(head == NULL){
		head = thread;
		tail = thread;
	}
	
	else{
		tail->next = thread;
		tail = thread;
	}
	
	#define STACKSIZE 128000
    unsigned char *stack = (unsigned char *)malloc(STACKSIZE);
    assert(stack);

    /* initialize thread */
    getcontext(&thread->ctx);
    thread->ctx.uc_stack.ss_sp   = stack;
    thread->ctx.uc_stack.ss_size = STACKSIZE;
    thread->ctx.uc_link          = &mainctx;
    makecontext(&thread->ctx, (void (*)(void))func, 1, arg);

    return;
}

void ta_yield(void) {

    return;
}

int ta_waitall(void) {
	
	listprint(head);
	
	node *runner = head;
	
	while(runner != NULL){
		if(swapcontext(&mainctx, &runner->ctx) == -1){
			fprintf(stderr,"WE\'RE GOING DOWN CAPTAIN\n");
			exit(1);
		}
		runner = runner->next;
	}
	
	listdestroy(head);
	
    return 0;
}
/* ***************************** 
     stage 2 library functions
   ***************************** *
void ta_sem_init(tasem_t *sem, int value) {
    sem->count = value;
    sem->sem_list = malloc(sizeof(struct node));
}
void ta_sem_destroy(tasem_t *sem) {
    while(sem->sem_list->next != NULL){
	list_append(&sem->sem_list->ctx, &list);
	struct node *temp = sem->sem_list;
	sem-> sem_list = sem->sem_list->next;
	free(temp);
    }
    free(sem);
}
void ta_sem_post(tasem_t *sem) {
	sem->count += 1;
	if((sem->count <= 0) && (sem->sem_list != NULL)){
	   swapcontext(&main_ctx, &sem->sem_list->ctx);
	}
}
void ta_sem_wait(tasem_t *sem) {
	if (sem->count > 0){
	   sem->count -=1;
	}
	if (sem == 0){
	   swapcontext(&main_ctx, &list->ctx);
	}
}
void ta_lock_init(talock_t *mutex) {
	mutex->sem = *(tasem_t*)malloc(sizeof(tasem_t));
	ta_sem_init(&mutex->sem,1);
}
void ta_lock_destroy(talock_t *mutex) {
	ta_sem_destroy(&mutex->sem);
	free(&mutex->sem);
}
void ta_lock(talock_t *mutex) {
	ta_sem_wait(&mutex->sem);
}
void ta_unlock(talock_t *mutex) {
	ta_sem_post(&mutex->sem);
}
*/