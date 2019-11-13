#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define quantum 2

struct data{
    int a;
    int b;
};

struct task
{
    void (*function)(void *p);
    void *data;
    int task_id;
};

struct queue {
    struct task *task;
    struct wait_queue *next;
    pthread_mutex_t mutex;
};

struct queue *wait_queue = NULL;
struct task *actual_work = NULL;
int index;

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;
    printf("a=%d b=%d",temp -> a, temp -> b);
}

void init_queue(void)
{
    if (wait_queue == NULL){
        wait_queue =(struct queue*)malloc(sizeof(struct queue));
        wait_queue -> task = NULL;
        wait_queue -> next = NULL;
        index = 0;
    }
}

struct task *create_task(void (*some_function)(void *p),void *p)
{
    pthread_mutex_lock(&wait_queue -> mutex);
    struct task *new_task = (struct task*)malloc(sizeof(struct task));
    new_task -> function = some_function;
    new_task -> data = p;
    new_task -> task_id = index;
    index++;
    return new_task;
}

void add_wait_queue(struct task *t)
{
    pthread_mutex_lock(&wait_queue -> mutex);
    if(wait_queue -> task == NULL){
        wait_queue -> task = t;
        wait_queue -> next = NULL;
        actual_work = t;
    }    
    else {
        struct queue *temp = wait_queue;
        while(temp -> next != NULL){
            temp = temp -> next;
        }
        temp -> next = t;
    }
    pthread_mutex_unlock(&wait_queue -> mutex);
}

void remove_queue(void)
{
    pthread_mutex_lock(&wait_queue -> mutex);
    if(actual_work == NULL){
        return FALSE;
    }
    else {
        wait_queue -> task = NULL;
        wait_queue = wait_queue -> next;
        actual_work = wait_queue -> task;
    }
    pthread_mutex_unlock(&wait_queue -> mutex);
}

void execute(void (*function)(void *p),void *p){
    (*function)(p);
}

int run(void *params){
    int thread_id = (int) params;
    struct task *actual_task;
    int num_of_works = 0;
    while(TRUE){
        if(num_of_works == quantum){
            return TRUE;
        }
        pthread_mutex_lock(&wait_queue -> mutex);
        if(wait_queue -> task == NULL){
            pthread_mutex_unlock(&wait_queue -> mutex);
        }
        else {
            remove_queue();
            ++num_of_works;
            execute(actual_work,actual_work -> data);
        }
    }
    pthread_exit(FALSE);
}

int main(void){
    struct data work;
    work.a = 10;
    work.b = 15;

    struct task task1 = create_task(&data,&add);

    return 0;
}