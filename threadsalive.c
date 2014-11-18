#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ucontext.h>
#include <errno.h>
#include "threadsalive.h"
#include "list.c"
#include "list.h"

/*
 * Jack Sneeringer and Michael Lese
 *
 * Jack did all of Stage 1.
 *
 * Mike attempted Stage 2 and Jack fixed all the seg faults
 * and cleaned up the code to use already defined functions
 *
 * Mike also attempted Stage 3.
 *
 * Jack wrote who was responsible for what.
 *
*/

/* ***************************** 
     stage 1 library functions
   ***************************** */

node *head;
node *tail;
ucontext_t mainctx;

//runordie error checks swapcontext
#define runordie(value) \
	if(value == -1){ \
		fprintf(stderr,"WE\'RE GOING DOWN CAPTAIN\n"); \
		printf("%s\n", strerror(errno)); \
		exit(1);} 

//This doesn't actually do anything?
void ta_libinit(void) {
	
    return;
}

//Adds a new context to the list and makes context
void ta_create(void (*func)(void *), void *arg) {
	
	addctx(&head,&tail,&mainctx);
    makecontext(&tail->ctx, (void (*)(void))func, 1, arg);

    return;
}

//Yields to next thread in list
void ta_yield(void) {
	
	if(head == NULL){
		return;
	}

	nextthread(&head,&tail);
	runordie(swapcontext(&tail->ctx,&head->ctx));
	
    return;
}

//While threads are still running swap to the thread
//and then destroy on completion
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

//Initialize the semaphore
void ta_sem_init(tasem_t *sem, int value) {
	
	sem->count = malloc(sizeof(int));
	sem->head = malloc(sizeof(node));
	sem->head->next = NULL;
	*(sem->count) = value;
	sem->tail = sem->head;
	
}

//Free the semaphore
void ta_sem_destroy(tasem_t *sem) {
	listdestroy(sem->head);
	free((sem->count));	
}

//Standard semaphore post without locks
void ta_sem_post(tasem_t *sem) {

	*(sem->count)+=1;

	if(*(sem->count) <= 0 && sem->head != NULL) {
		nextthread(&(sem->head),&(sem->tail));
		runordie(swapcontext(&sem->tail->ctx,&sem->head->ctx));
	}
}

//Standard semaphore wait without locks
void ta_sem_wait(tasem_t *sem) {
	
	if(*(sem->count) <= 0 && sem->head != NULL) {
		nextthread(&(sem->head),&(sem->tail));
		runordie(swapcontext(&sem->tail->ctx,&sem->head->ctx));
	}
	*(sem->count) -= 1;
}

//Call semaphore with value of one
void ta_lock_init(talock_t *mutex) {
	mutex->sem = malloc(sizeof(tasem_t));
	ta_sem_init(mutex->sem,1);
}

//Free lock
void ta_lock_destroy(talock_t *mutex) {
	ta_sem_destroy(mutex->sem);
	free(mutex->sem);
}

//Lock
void ta_lock(talock_t *mutex) {
	ta_sem_wait(mutex->sem);
}

//Unlock
void ta_unlock(talock_t *mutex) {
	ta_sem_post(mutex->sem);
}

/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
	cond->sem = malloc(sizeof(tasem_t));
	ta_sem_init(cond->sem,0);

}

void ta_cond_destroy(tacond_t *cond) {
	ta_sem_destroy(cond->sem);
}

void ta_wait(talock_t *lock, tacond_t *cond) {
	ta_unlock(lock);
	ta_sem_wait(cond->sem);
	ta_lock(lock);
}

void ta_signal(tacond_t *cond) {
	ta_sem_post(cond->sem);
}








