#include <stdio.h>
#include <stdlib.h>

#define DEBUG 2 //1 ou 2

#define PAGE_TABLE_SIZE 128
#define VALID 1
#define INVALID 0

#define NONE -1

#define TRUE 1
#define FALSE 0

//Page table related Node
typedef struct{
    int frame;
    int valid;
}Node;

//Physical memory related Node
typedef struct{
    char content [256];
}PhyNode;

//Logical Address
typedef struct{
    int page;
    char offset[16];
}LogAdd;

//Basic counters
int page_hit_count;
int page_fault_count;

//Main memory
PhyNode mainMem[PAGE_TABLE_SIZE];

//Page table
Node pageTable [PAGE_TABLE_SIZE];

//Primitive ops in pageTable
void init_page_table()
{
    for(int i = 0; i < PAGE_TABLE_SIZE;i++){
        pageTable[i].frame = -1;
        pageTable[i].valid = INVALID;
    }
}

void printPageTable() 
{
    printf("\nPAGE\tFRAME\tSTATE");
    for(int i = 0;i < PAGE_TABLE_SIZE;i++){
        char *mes = (pageTable[i].valid ? "VALID" : "INVALID");
        printf("\n%d\t| %d\t| %s \t|",i,pageTable[i].frame,mes);
    }
}

int req_page(int page,int frame)
{
    int result = FALSE;
    if(!pageTable[page].valid){
        pageTable[page].valid = VALID;
        pageTable[page].frame = frame;
        result = TRUE;
        page_fault_count++;
    }
    page_hit_count++;
    return result;
}


int release_page(int page)
{
    int result = FALSE;
    if(pageTable[page].valid) {
        pageTable[page].valid = INVALID;
        pageTable[page].frame = NONE;
        result = TRUE;
    }
    return result;
}

// void logical_interpreteer(LogAdd logAdd) 
// {
//     int page = logAdd.page;
//     char *offset = logAdd.offset;
//     if(req_page(page,)) {
//         int frame = pageTable[page];
//         PhyNode *physical = mainMem[offset];
//     }
// }

//Main memory simulation

void init_main_memory()
{
    for(int i = 0; i < PAGE_TABLE_SIZE;i++) {
        for(int j = 0;j < PAGE_TABLE_SIZE;j++) {
            mainMem[i].content[j] = '0';
        }
    }
}

void printMainMemory()
{
    for(int i = 0;i < PAGE_TABLE_SIZE;i++){
        printf("\nFRAME: %d\t|",i);
        for(int j = 0;j < PAGE_TABLE_SIZE;j++){
            printf("[%c]",mainMem[i].content[j]);
        }
    }
}

void req_frame(int frame,int content)
{

}


int main(void) 
{
    page_hit_count = 0;
    page_fault_count = 0;
    init_page_table();
    init_main_memory();
    req_page(10,20);
    if(DEBUG>0){
        printPageTable();
    }
    if(DEBUG>1){
         printMainMemory();
    }

    printf("\nPage Faults = %d",page_fault_count);
    return 0;
}