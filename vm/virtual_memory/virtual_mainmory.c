#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "support.h"

#define FISICAL_SIZE 128
#define PAGE_SIZE 256

#define DEBUG 1

char fisicalMemory [FISICAL_SIZE][PAGE_SIZE];

int virtual_memory_alloc(struct paging * address);
struct Node * not_full(struct paging * address);
void writeToFisical (struct Node * fisical);


int main(int argc, char const *argv[])
{
    int *binaryAddress;
    int logicalAddress;
    struct paging address;
    FILE *input;
    binaryAddress = malloc(32*sizeof(int));

    createPageTable();
    initTlb();
    
    input = fopen(argv[1], "r");
    while(fscanf(input, "%d", &logicalAddress) > 0){
        intToBinary(logicalAddress, binaryAddress);
        address = translateAddress(binaryAddress);
        int frame = virtual_memory_alloc(&address);
        int fis = frame * address.offset;
        printf("Virtual address: %d Physical address: %d Value: %d\n", logicalAddress,fis, fisicalMemory[frame][address.offset]);
        //searchAddress(address);
    }
    free(binaryAddress);
    fclose(input);

    return 0;
}

int virtual_memory_alloc(struct paging * address){
	if(DEBUG == 1) {
		printf("\n[CALL]-virtual_memory_alloc\n");
	}
	int tlb_add = tlbShoot(address->page);

	if(tlb_add == -1){//Caso erre a tlb
		struct Node * available = not_full(address);

		if(available == NULL){//Erre a tlb e a Page Table
			available = lru();
			writeToFisical(available);
			tlbPush(available);
			return available->frame;
		}
		writeToFisical(available);
		tlbPush(available);
		return available->frame;
	}

	return tlb_add;
}


struct Node * not_full(struct paging * address){
	struct Node * current = pageTable->first;

	while(current->next != NULL){
		if(current->page == -1){
			current->page = address->page;
			writeToFisical(current);
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void writeToFisical (struct Node * fisical){
	long int startPos;
	int wordSize = 8;
	char buffer[PAGE_SIZE];
	FILE *binary;

	binary = fopen("BACKING_STORE.bin", "r");

	startPos = PAGE_SIZE * (long int)fisical->page;
	fseek(binary, startPos, SEEK_SET);
	fread(buffer,sizeof(buffer),1,binary);

	for (int i = 0; i < PAGE_SIZE; i++){
		fisicalMemory[fisical->frame][i] = buffer[i];
	}

	fclose(binary);
}






