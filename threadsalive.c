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

#define runordie(value) \
	if(value == -1){ \
		fprintf(stderr,"WE\'RE GOING DOWN CAPTAIN\n"); \
		exit(1);} 

void ta_libinit(void) {
	
    return;
}

void ta_create(void (*func)(void *), void *arg) {
	
	addctx(&head,&tail,&mainctx);
    makecontext(&tail->ctx, (void (*)(void))func, 1, arg);

    return;
}

void ta_yield(void) {
	
	if(head == NULL){
		return;
	}

	nextthread(&head,&tail);
	runordie(swapcontext(&tail->ctx,&head->ctx));
	
    return;
}

int ta_waitall(void) {
	
	while(head != NULL){
		runordie(swapcontext(&mainctx, &head->ctx))
		headdestroy(&head);
	}
	
    return 0;
}

/* ***************************** 
     stage 2 library functions
   ***************************** */
void ta_sem_init(tasem_t *sem, int value) {
	sem->count = malloc(sizeof(int));
	sem->head = malloc(sizeof(node));
	sem->tail = head;
}
void ta_sem_destroy(tasem_t *sem) {
	listdestroy(sem->head);
	free((sem->count));	
}

void swaphandler(tasem_t *sem) {
	ucontext_t former;
	addctx(&(sem->head),&(sem->tail),&former);
	runordie(swapcontext(&former, listremove(&(sem->head))));
}

void ta_sem_post(tasem_t *sem) {
	sem->count+=1;

	if(sem->count <= 0 && sem->head != NULL) {
		swaphandler(sem);
	}
}

void ta_sem_wait(tasem_t *sem) {
	if(sem->count == 0) {
		swaphandler(sem);
	}
	if(sem->count > 0) {
		sem->count -= 1;
	}
}

void ta_lock_init(talock_t *mutex) {
	mutex->sem = malloc(sizeof(tasem_t));
	ta_sem_init(mutex->sem,1);
}
void ta_lock_destroy(talock_t *mutex) {
	ta_sem_destroy(mutex->sem);
	free(mutex);
}
void ta_lock(talock_t *mutex) {
	ta_sem_wait(mutex->sem);
}
void ta_unlock(talock_t *mutex) {
	ta_sem_post(mutex->sem);
}















