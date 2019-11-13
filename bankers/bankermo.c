#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

struct task {
    int task_id;
    int **resources_reqs;
    int awake;
    struct task *next;
};

struct session {
    struct task *task_line;
    int task_counts;
    int semaphor;
    int **available_resources;
};

int generate_resources(){
    int actual_resource = 0;
    while(!(actual_resource > 0) && !(actual_resource < 0)){
        actual_resource = rand();
    }
    return actual_resource;
}

int main(int argc, char const *argv[]){
    int actual = generate_resources();
    printf("%d",actual);
    return 0;
}