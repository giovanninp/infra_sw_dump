/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10 // not used in this code
#define NUMBER_OF_THREADS 3

#define TRUE 1
#define DEBUG 2 // 0 - no debug message, 1 - main points, 2 - all

struct task worktodo;
struct threadpool *pool;
time_t begin;

pthread_t bee; // the worker bee

int i = 0; // task_id debugging
int j = 0; // thread_id debugging

// insert a task into the queue
int enqueue(struct task *t) 
{
    if (pool->head == NULL) { // pool is currently empty
        if (DEBUG > 1) printf("Pool is currently empty...\n");
        pool->head = t;
        pool->tail = t;
    } else {
        if (DEBUG > 1) printf("Pool isn't currently empty...\n");
        pool->tail->next = t;
        pool->tail = t;
    }
    if (DEBUG > 1) printf("Task with id = %d added...\n", t->task_id);
    return 0;
}

// remove a task from the queue
struct task* dequeue() 
{
    struct task *worktodo;
    worktodo = pool->head;
    if (worktodo == NULL) { // dequeue was required in an empty thread
        return NULL;
    }
    if (worktodo->next == NULL) { // dequeue made pool empty
        pool->head = NULL;
        pool->tail = NULL;
    } else {
        pool->head = worktodo->next;
    }
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    int thread_id = (int) param;
    if (DEBUG) printf("Start work of thread %u...\n", thread_id);
    struct task *task;
    int num_of_works = 0;
    time_t now;
    while (TRUE) {
        now = time(NULL);
        sem_wait(&pool->semaphore);
        pthread_mutex_lock(&pool->mutex);
        if (DEBUG && pool->head) printf(".locking in thread %u...\n", thread_id);
        if (pool->head == NULL) {
            pthread_mutex_unlock(&pool->mutex);
        } else {
            task = dequeue();
            ++num_of_works;
            if (DEBUG > 1) printf("Thread %u got job of adding %u + %u...\n", thread_id, *((int*) task->data), *((int*) task->data + 1));
            pthread_mutex_unlock(&pool->mutex);
            if (DEBUG) printf(".unlocking in thread %u...\n", thread_id);
            execute(task->function, task->data);
        }
        sem_post(&pool->semaphore);
    }
    if (DEBUG) printf("Closing thread %u, that performed %u works...\n",thread_id, num_of_works);
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
 //Receiving a function, in that case, add, and a data struct
 //int pool_submit(&add,&workn);
int pool_submit(void (*somefunction)(void *p), void *p)
{
    pthread_mutex_lock(&pool->mutex); //Locking other processes to run the critical section
    if (DEBUG > 1) printf("New task in the pool... Locking\n");
    struct task *worktodo = (struct task*) malloc(sizeof(struct task)); //Allocating a new task struct
    worktodo->function = somefunction;
    worktodo->data = p;
    worktodo->task_id = i;
    worktodo->next = NULL;
    ++i;//Increasing task_id
    if (DEBUG > 1) {
        printf("work.a = %u ... ", *((int*) worktodo->data)); 
        printf("work.b = %u\n", *((int*) worktodo->data + 1));
        //printf(">>> *((int*) worktodo.data) == worktodo.b = %u\n", *((int*) (worktodo.data + sizeof(int))));
    }
    enqueue(worktodo);
    if (DEBUG > 1) printf("Enqueued. Unlocking execution...\n");
    sem_post(&pool->semaphore);
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    pool = (struct threadpool*) malloc(sizeof(struct threadpool));
    pool->head = NULL;
    pool->tail = NULL;
    pthread_mutex_init(&pool->mutex, NULL);
    sem_init(&pool->semaphore, 0, NUMBER_OF_THREADS);
    begin = time(NULL);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i, ++j) {
        pthread_create(&bee, NULL, worker, (void *) j);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
    if (DEBUG) printf("End of execution :)\n");
}