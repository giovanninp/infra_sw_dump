#include <stdio.h>
#include <stdlib.h>

#define PAGE_TABLE_LENGTH 128

#define TRUE 1
#define FALSE 0

#define AVAILABLE 1
#define UNAVAILABLE 0

#define DEBUG 0 //1 ou 2

typedef struct{
    int page;
    int displacement;
}LogAdd;

typedef struct{
    int frame;
    int displacement;
}PhysAdd;

typedef struct {
    int frame;
    int available;
}PageNode;

typedef struct {
    int displacement;
}MainNode;

PageNode pageTable [128];
PageNode mainMemory [128];

int page_hit_count;
int page_fault_count;

void init_page_table() 
{
    for(int i = 0; i < PAGE_TABLE_LENGTH;i++){
        pageTable[i].available = TRUE;
        pageTable[i].frame = -1;
    }
}

int check_page_availability(int page)
{
    int result = FALSE;
    if(pageTable[page].available){
        result = true;
    }
    return result;
}

int req_page(int index,int frame)
{
    int result = -1;
    if(index >= 0 && index < PAGE_TABLE_LENGTH) {
        if(!check_page_availability(index)){
            result = pageTable[index].frame;
            page_hit_count++;
        }
        else page_fault_count++;
    }
    return result;
}

MainNode get_physical_address(LogAdd logicalAddress)
{
    

}


int main()
{
    return 0;
}