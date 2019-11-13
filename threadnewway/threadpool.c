#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0

#define NUMBER_OF_THREADS 3
#define LIFE 2

pthread_t bee;

struct data{
    int a;
    int b;
};

struct task {
    void (*function)(void *p);
    void *data;
    struct task *next;
    int task_id;
};

struct threadpool{
    struct task *head;
    struct task *tail;
    pthread_mutex_t mutex;
    sem_t semaphore;
    int timespan;
};

struct task worktodo;
struct threadpool *pool;
time_t begin;

int enqueue(struct task *t){
    if(pool -> head == NULL){
        pool -> head = t;
        pool -> tail = t;
    }
    else {
        pool -> tail -> next = t;
        pool -> tail = t;
    }
    sem_post(&pool -> semaphore);
}

void pool_init(void)
{

}

int main(void)
{
    return 0;
}