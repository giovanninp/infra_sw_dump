#ifndef SUPPORT
#define SUPPORT

struct DoubleLinked{
	struct Node * first;
	struct Node * last;
};
struct Pager{
	int page;
	int frame;
};
struct Node{
	struct Node * next;
	int page;
	int frame;
	struct Node * previous;
};
struct paging{
	int page;
	int offset;
};

struct DoubleLinked * pageTable;

//LRU.c
//--------
void createPageTable();
void printTable(struct Node * current,int n);
void cleanPageTable();
void freeList(struct Node * current);
void lruStack(struct Node * toStack);
struct Node * lru();
//--------
//tlb.c
//--------
void initTlb();
void printTlb();
int tlbShoot(int page);
void * tlb_runner(void * arg);
void tlbPush(struct Node * toInsert);
//--------
//input_read
void intToBinary(int n, int *pointer);
struct paging translateAddress(int *pointer);
void searchAddress(struct paging address);
int twoToPow(int power);

#endif