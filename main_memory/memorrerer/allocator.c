#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"allocator.h"

/*
 *
 * It is still missing what how the memory  
 * will be simulated. Maybe will be use 
 * Linked List
 *
 */

int main(int argc, char const *argv[])
{	
	struct memory_allocation* memory = (struct memory_allocation*) malloc(sizeof(struct memory_allocation));

	memory -> id = 0;
	memory -> first_pos = 0;
	memory -> last_pos = atoi(argv[1]);
	memory -> used = 0;
	int size = atoi(argv[1]);

	while(TRUE){
		printf("allocator> ");
		char operation[32];
		scanf("%s", operation);

		if(!strcmp(operation,"X")){
			break;

		} else if(!strcmp(operation,"STAT")){
			
			// make stat operation
			stat_operation(memory);

		} else if(!strcmp(operation, "C")){
			
			// compact operation
			struct memory_allocation* result= (struct memory_allocation*) malloc(sizeof(struct memory_allocation));
			memory = compaction_operation(memory,result,size);

		} else if(!strcmp(operation, "RL")){

			release_operation(memory);
			defrag_unused(memory);
		
		} else if(!strcmp(operation, "RQ")){
		
			choosing_algorithm(memory);
		
		} else {
			printf("operation not find\n");
		}
	}
	return 0;
}

void choosing_algorithm(struct memory_allocation* memory)
{
	char process_id[20], algorithm[2];
	int size;
	scanf("%s", process_id);
	scanf("%d", &size);
	scanf("%s", algorithm);
	if(algorithm[0] ==  'F')
		memory = first_alloc_memory(memory, size, process_id);
	else if(algorithm[0] ==  'W')
		worst_alloc_memory(memory, size, process_id);
	else if(algorithm[0] == 'B')
		best_alloc_memory(memory, size, process_id);
}

// everytime this function is called it loses some memory
void release_operation(struct memory_allocation* head) 
{
	char process_id[20];
	struct memory_allocation* current = head;
	scanf("%s", process_id);
	while(current -> next != NULL){
		if(!strcmp(current -> process, process_id)){
			current -> used = FALSE;
			printf("Process %s was released\n", current -> process);
			break;
		}
		current = current ->next;
	}
	if (current -> next != NULL && current -> next -> used == FALSE){
		struct memory_allocation* temp = current -> next;
		current -> last_pos = temp -> last_pos;
		current -> next = temp -> next;
		free(temp);
	}
}

// algorithm to put in  the first free space
struct memory_allocation* first_alloc_memory(struct memory_allocation* head, int size, char process[20])
{
	struct memory_allocation* memory = head;
	while(TRUE){
		
		if((memory ->last_pos - memory -> first_pos) >= size && memory -> used == FALSE){
			break;

		} else if(memory -> next == NULL){
			printf("Não tem espaço\n");
			return memory;

		}
		memory = memory -> next;
	}
	strcpy(memory -> process, process);
	memory -> used =TRUE;
	int space = memory -> last_pos - memory -> first_pos; 
	if(space > 0){

		unused_space(memory, size);
	}
	printf("%s %d %d\n", memory -> process, memory -> first_pos, memory -> last_pos);
	return memory;
}


void unused_space(struct memory_allocation* memory, int size)
{
	struct memory_allocation* memory_new = (struct memory_allocation*) malloc(sizeof(struct memory_allocation));
	memory_new -> used = FALSE;
	memory_new -> id = memory -> id +1;
	memory_new -> last_pos = memory -> last_pos;
	memory -> last_pos = memory -> first_pos + size;
	memory_new -> first_pos = memory -> last_pos+1;
	memory_new -> next = memory -> next;
	memory -> next = memory_new;

	// if(memory_new -> next != NULL){
	// 	memory_new -> next -> used = FALSE;
	// 	memory_new -> next -> first_pos = memory_new -> last_pos + 1;
	// }
}

int worst_alloc_memory(struct memory_allocation* head, int size, char process[20])
{
	int max = find_max(head, size);
	if(max < 0)
		return 0;

	struct memory_allocation* current = find_process(head, max);
	strcpy(current -> process, process);

	current -> used = TRUE;
	unused_space(current, size);
}
int best_alloc_memory(struct memory_allocation* head, int size, char process[20])
{
	int min = find_min(head, size);
	if(min < 0)
		return 0;

	struct memory_allocation* current = find_process(head, min);
	strcpy(current -> process, process);

	current -> used = TRUE;
	unused_space(current, size);
	
}

struct memory_allocation* compaction_operation(struct memory_allocation* head,struct memory_allocation* result, int size) 
{
	struct memory_allocation* current = (struct memory_allocation*) malloc(sizeof(struct memory_allocation));
	result  -> next=current;
	int last_pos = 0;
	while(head != NULL) {
		if(head ->used == TRUE){			
			if(last_pos != 0) {
				printf("AQUI\n");
				struct memory_allocation* temp= (struct memory_allocation*) malloc(sizeof(struct memory_allocation));
				strcpy(temp->process, head->process);
				temp ->id = head->id;
				temp -> last_pos =  head -> last_pos - head -> first_pos + last_pos;
				temp -> first_pos = last_pos;
				temp -> used = TRUE;
				// temp -> next = NULL;
				current -> next = temp;
				current = current -> next;
			} else{
				strcpy(current->process, head->process);
				current ->id = head->id;
				current -> last_pos =  head -> last_pos - head -> first_pos + last_pos;
				printf("AQUI2\n");
				current -> first_pos = last_pos;
				current -> used = TRUE;
				// current -> next = NULL;
			}
			last_pos = current -> last_pos + 1;
			printf("Adress  ");
			printf("[%d:%d] %d \n", current->first_pos, current->last_pos,last_pos);
		}
		head = head -> next;
	}	
	struct memory_allocation* temp= (struct memory_allocation*) malloc(sizeof(struct memory_allocation));
	temp -> last_pos =  size;
	temp -> first_pos = last_pos;
	temp -> used = FALSE;
	temp -> next = NULL;
	current -> next = temp ;		 
	printf("Adress  ");
	printf("[%d:%d]  \n", current->first_pos, current->last_pos);
	printf("%d", last_pos);
	return (result  -> next);
	// faz sentindo ainda ter esses algorimtos de memoria com o barateamento da memoria

}

void defrag_unused(struct memory_allocation* head)
{
	struct memory_allocation* memory = head;
	while(memory -> next != NULL){
		if(memory -> used == FALSE && memory ->next -> used == FALSE){
			struct memory_allocation* next = head -> next;
			memory -> last_pos = next -> last_pos;
			memory -> next = next -> next;
		} 
		memory = memory -> next;
		
	}
}

void stat_operation(struct memory_allocation* memory) 
{	
	while(memory != NULL){
		printf("Adress  ");
		printf("[%d:%d]  ", memory->first_pos, memory->last_pos);
		if(memory->used)
			printf("Process %s\n", memory->process);
		else
			printf("Unused\n");
		memory=memory->next;
	}
}

int find_max(struct memory_allocation* head, int size)
{
	struct memory_allocation* current = head;
	int max = 0;
	int id = -1;
	while(current != NULL){
		int space = current -> last_pos - current -> first_pos;
		if(max < space && size <= space && current-> used == FALSE){
			max = (current -> last_pos - current -> first_pos);
			id = current -> id;
		}

		current = current ->next;
	}
	return id;
}
int find_min(struct memory_allocation* head, int size)
{
	struct memory_allocation* current = head;
	int min = 1000000000;
	int id = -1;
	
	while(current != NULL){
		int space = current -> last_pos - current -> first_pos;
		if(min > space && size <= space && current-> used == FALSE){
			min = (current -> last_pos - current -> first_pos);
			id = current -> id;
		}

		current = current ->next;
	}
	return id;
}

struct memory_allocation* find_process(struct memory_allocation *head,  int id)
{	
	struct memory_allocation* current = head;
	while(current -> id != id){
		current = current -> next;
	}
	return current;
}