#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mutex.h   >

struct task {
    int id;
    pthread_t *thread;
};

struct task_pool{
   struct task *actual_task;
   struct task *next_task; 
};

struct task_pool *createTaskPool(){
    struct task_pool *pool = (struct task_pool*)malloc(sizeof(struct task_pool));
    pool -> actual_task = NULL;
    pool -> next_task = NULL;
    return pool;
}

pthread_t *createTask(void *function,task_pool) {

}


int main(void) {
    struct task_pool *pool = createTaskPool();
    free(pool);
    return 0;
};