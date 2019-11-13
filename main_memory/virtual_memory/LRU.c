#include <stdlib.h>
#include <stdio.h>
#include "support.h"

#define PAGE_TABLE_SIZE 128
#define INIT_PAGE_NUMBER -1

//Users/concretekite/Documents/Programs/C/OS/VM/virtual_memory

struct DoubleLinked * pageTable;

// int main(int argc, char const *argv[]){//Testes
// 	createPageTable();
// 	struct Node * next = pageTable->first;
// 	next = next->next;
// 	next = next->next;
// 	lruStack(next);
// 	printTable(pageTable->first,0);
// 	struct Node * toPrint = lru();
// 	printf("LRU Node: %d %d\n",toPrint->page, toPrint->frame);
// 	cleanPageTable();
// 	return 0;
// }

struct Node * lru(){
	lruStack(pageTable->last);
	return pageTable->first;
}

//Pega pagina e a coloca no topo da page table como a mais recente
void lruStack(struct Node * toStack){
	if(toStack == pageTable->first){
		return;
	}else if(toStack == pageTable->last){
		struct Node * pre = toStack->previous;
		pre->next = NULL;
		pageTable->last = toStack->previous;
		toStack->previous = NULL;
		toStack->next = pageTable->first;
		pageTable->first = toStack;

	}else{
		struct Node * pre = toStack->previous;
		struct Node * nxt = toStack->next;
		pre->next = nxt;
		nxt->previous = pre;
		toStack->next = pageTable->first;
		pageTable->first = toStack;
		toStack->previous = NULL;
		return;
	}
}

//Cria a Page Table
void createPageTable(){

	pageTable = (struct DoubleLinked *)malloc(sizeof(struct DoubleLinked));
	pageTable->first = (struct Node *)malloc(sizeof(struct Node));
	pageTable->first->previous = NULL;
	pageTable->first->page = INIT_PAGE_NUMBER;
	pageTable->first->frame = 0;// Alteração
	pageTable->last = NULL;

	struct Node * current = pageTable->first;
	struct Node * new;
	for (int i = 1; i < PAGE_TABLE_SIZE; i++){
		new = (struct Node*)malloc(sizeof(struct Node));
		current->next = new;
		new->previous = current;
		new->page = INIT_PAGE_NUMBER;
		new->frame = i;//Alteração
		current = new;
	}

	current->next = NULL;
	pageTable->last = current;

}

//Limpa a Memoria
void cleanPageTable(){
	freeList(pageTable->first);
	free(pageTable);
}
//Função Auxiliar
void freeList(struct Node * current){
	if(current == NULL){
		return;
	}else{
		struct Node * next = current->next;
		free(current);
		freeList(next);
	}
}
//Função de teste
void printTable(struct Node * current , int n){
	if(current == NULL){
		return;
	}else{
		printf("Node %d: %d %d\n",n, current->page, current->frame);
		printTable(current->next, ++n);
	}
}
