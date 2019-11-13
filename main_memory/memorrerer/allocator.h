#define TRUE 1
#define FALSE 0

struct memory_allocation{
	int id;
	char process[20];
	int first_pos;
	int last_pos;
	int res_memory;
	int used;
	struct memory_allocation *next;
};

void choosing_algorithm(struct memory_allocation* memory);
void release_operation(struct memory_allocation* memory);
void stat_operation(struct memory_allocation* memory);
struct memory_allocation* first_alloc_memory(struct memory_allocation* head, int size, char *process);
struct memory_allocation*  compaction_operation(struct memory_allocation* head,struct memory_allocation* result, int size) ;
struct memory_allocation* find_process(struct memory_allocation *head, int id);
int find_max(struct memory_allocation* head, int size);
int worst_alloc_memory(struct memory_allocation* head, int size, char process[20]);
void unused_space(struct memory_allocation* memory, int size);
int best_alloc_memory(struct memory_allocation* head, int size, char process[20]);
int find_min(struct memory_allocation* head, int size);
void defrag_unused(struct memory_allocation* head);