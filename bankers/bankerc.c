#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define true 1
#define false 0

#define MAX_TIME 10

#define NUMBER_OF_RESOURCES 3
#define NUMBER_OF_CUSTOMERS 4

struct client {
    int resource [NUMBER_OF_RESOURCES];
    int cid;
};

struct line {
    struct client actual_client;
    struct line *next;
};

struct session{
    struct line *clients_line;
    int *resources;
    int clients_count;
};

int request_resource(struct client *client,struct session *session){
    int result = true;


    return result;
};

int release_resource(){
    int result = true;
    return result;
};

int *copy_array(int *array1){
    int result[NUMBER_OF_RESOURCES];
    for(int i = 0;i < NUMBER_OF_RESOURCES;i++){
        result[i] = array1[i];
    }
    return result;
};

struct session *create_session(void *resources){
    struct session *new_session;
    new_session = (struct session*)malloc(sizeof(struct session));
    new_session -> resources = (int *) resources;
    new_session -> clients_count = 0;
    new_session -> clients_line = (struct line *)malloc(sizeof(struct line));
    return new_session;
};

struct generate_client(struct session *actual){
    return NULL;
};

int main(int *resources) {
    int resources[][] = {{1,2,3},{1,2,3}};

    struct session *actual_session = create_session();
    printf("%d",actual_session->clients_count);
    return 0;
};