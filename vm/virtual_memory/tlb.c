#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "support.h"

#define TLB_SIZE 16

//TLB implementada como fifo

struct Pager * tlb[TLB_SIZE];
int thread_frame;
int page_search;

#define DEBUG 2

// int main(int argc, char const *argv[]){
// 	initTlb();
// 	struct Pager tests[17];
// 	for (int i = 0; i < 17; i++){
// 		tests[i].page = (i * 2);
// 		tests[i].frame = i;
// 		tlbPush(&tests[i]);
// 		//printTlb();
// 	}
// 	printf("Hit at %d\n", tlbShoot(18));
// 	printTlb();
// 	return 0;
// }

//Vê se a pagina esta na TLB caso sim retorna o valor do frame caso não retorna -1;
int tlbShoot(int page){
	if(DEBUG == 1) {
		printf("\n[CALL]-tlbShoot\n");
	}
	thread_frame = -1;
	page_search = page;
	// pthread_t tids[TLB_SIZE];
	for (int i = 0; i < TLB_SIZE; i++){
		tlb[i];
		// pthread_attr_t attr;
		// pthread_attr_init(&attr);
		// pthread_create(&tids[i], &attr, tlb_runner, (void *)tlb[i]);
	}
	for (int i = 0; i < TLB_SIZE; i++){
		// pthread_join(tids[i], NULL);
	}
	return thread_frame;
}

//Função auxiliar
void * tlb_runner(void * arg){
	if(DEBUG == 1) {
		printf("\n[CALL]-tlb_runner\n");
	}
	struct Pager * search = (struct Pager*)arg;
	if(page_search == search->page){
		thread_frame = search->frame;
	}
	pthread_exit(0);

}

//Recebe um ponteiro para nodulo da page table Incere valor de pagina e frame na tlb 
void tlbPush(struct Node * toInsert){
	if(DEBUG > 0) {
		printf("\n[CALL]-tlbPush\n");
	}
	for (int i = 0; i < TLB_SIZE; i++){
		if(tlb[i]->frame < 0){
			if(DEBUG > 1) {
				printf("\n\t[tlbPush] - frame < 0\n");
			}
			tlb[i]->page = toInsert->page;
			tlb[i]->frame = toInsert->frame;
			return;
		}
	}
	for (int i = TLB_SIZE - 1 ; i > 0 ; i--){
		tlb[i]->page = tlb[i - 1]->page;
		tlb[i]->frame = tlb[i - 1]->frame;
	}
	tlb[0]->page = toInsert->page;
	tlb[0]->frame = toInsert->frame;
}

//Cria a TLB
void initTlb(){
	if(DEBUG == 1) {
		printf("\n[CALL]-initTlb\n");
	}
	for (int i = 0; i < TLB_SIZE; i++){
		tlb[i] = (struct Pager*)malloc(sizeof(struct Pager));
		tlb[i]->page = -1;
		tlb[i]->frame = -1;
	}
}

void printTlb(){
	if(DEBUG == 1) {
		printf("\n[CALL]-printTlb\n");
	}
	for (int i = 0; i < TLB_SIZE; i++){
		printf("TLB %d: %d %d\n",i, tlb[i]->page, tlb[i]->frame);
	}
}