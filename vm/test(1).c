#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_TABLE_SIZE 256
#define PHYSICAL_SIZE 128
#define TLB_SIZE 16

typedef struct {
	int page;
	int offset;
}paging;
  
typedef struct {
	int page;
	int frame;
}pager;

typedef struct{
	int frame;
	int offset;
}physical;

typedef struct{
	int page;
	int offset;
}tlb;


//inicio do decode 

paging translateAddress (int *pointer)
{
    paging translated;
    int result = 0;
    int power = 0;
    
    for (int i = 31; i > 15; i--) {
        if (i == 23) {
            translated.offset = result;
            result = 0;
            power = 0;
        }
        
        result += pointer[i] * twoToPow(power);
        power++;
    }

    translated.page = result;
    return translated;
}

void intToBinary(int number, int *pointer)
{
    int test, index = 0;
 
    for (int i = 31; i >= 0; i--) {
        test = number >> i;
 
        if (test & 1)
            pointer[index] = 1;
        else
            pointer[index] = 0;

        index++;
    }
}

int twoToPow(int power)
{
    int result = 1;

    for (int i = 1; i <= power; i++)
        result = result * 2;

    return result;
}


void printar(pager *pageTable)
{
    printf("Searching page %d with frame %d\n", pageTable->page, pageTable->frame);
}

//fim do decode

void createpageTable(pager *pageTable,paging address,int index){
	pageTable = malloc(sizeof(PAGE_TABLE_SIZE)); 
	pageTable->page= address.page;
	pageTable->frame = index;
	index++; 
}

// void createPhysicalMemory(pager pageTable[],paging address){
// 	physical *add = malloc(sizeof(PHYSICAL_SIZE));
// 	add->frame = pageTable
// }

// void createTlb(){
// 	tlb TLB[TLB_SIZE];
// 	tlb
// }


int main(int argc, char const *argv[])
{
	pager *now;
    int *binaryAddress;
    int logicalAddress;
    paging address;
    FILE *input;

    binaryAddress = malloc(32*sizeof(int));

    input = fopen(argv[1], "r");
    
    while(fscanf(input, "%d", &logicalAddress) > 0){
        intToBinary(logicalAddress, binaryAddress);
        address = translateAddress(binaryAddress);
        printar(now);
		createpageTable(now,address,0);
    }
    free(binaryAddress);
    fclose(input);

    return 0;
}
