/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool

typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;

struct node {
    task job;
    struct node *next;
};

struct node *head = NULL;

// the worker bee
pthread_t bee;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    struct node *new_task = (struct node*)malloc(sizeof(struct node));
    new_task->job = t;

    if (head == NULL) {
        head = new_task;
    } else {
        struct node *current = head;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = new_task;
        new_task->next = NULL;
    }

    return 0;
}

// remove a task from the queue
task dequeue() 
{
    worktodo = head->job;
    struct node *oldhead = head;
    head = head->next;
    free(oldhead);
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    enqueue(worktodo);

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_create(&bee,NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}