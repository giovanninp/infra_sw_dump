#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define true 1
#define false 0

#define empty -1
#define to_remove -2
//Structs:------------------------------------------------------------------

struct space {
    int id;
    char *process;
    int required_size;
    int init;
    int end;
    struct space *next;
};

struct  memory {
    int size;
    int free;
    int space_count;
    struct space *spaces;
};

struct command {
    int operation;
    int value;
    char *process;
};

struct space *create_space(int id,char *process, int required_size);

//Memory:---------------------------------------------------------------------

struct memory *create_memory(int size)
{
    struct memory *new_memory = (struct memory*)malloc(sizeof(struct memory));
    new_memory -> size = size;
    new_memory ->free = size;
    new_memory -> spaces = NULL;
    return new_memory;
}

struct memory *init_main_memory(int size)
{
    struct memory *main_memory = create_memory(size);
    main_memory -> spaces = create_space(empty,NULL,size);
    main_memory -> spaces -> init = 1;
    main_memory -> spaces -> end = size;
    main_memory -> space_count = 1;
    return main_memory;
}

//Process:---------------------------------------------------------------------

struct space *create_space(int id,char *process, int required_size)
{
    struct space *new_space = (struct space*)malloc(sizeof(struct space));
    new_space -> id = id;
    new_space -> process = process;
    new_space -> required_size = required_size;
    new_space -> next = NULL;
    return new_space;
}

int unllocate_space(struct memory *main_memory,int id){
    int result = false;
    if(id != empty){
        struct space *temp = main_memory -> spaces;
        while(temp != NULL){
            if(temp -> id == id){
                break;
            }
        }
        temp -> id = empty;
        temp -> process = NULL;
        result = true;
        main_memory -> free += temp -> required_size;
    }
    return result;
}

//Allocation alogrithms:-------------------------------------------------------

int ff_allocation(struct memory *main_memory,struct space *process)
{
    if(main_memory -> size < process -> required_size){
        printf("|ERRO - (MEMORIA INSUFICIENTE)");
        return false;
    }

    printf("\nFF-Allocation\n");
    struct space *temp = main_memory -> spaces;
    struct space *prev = NULL;
    struct space *next = NULL;
    int count = 0;
    while(temp -> next != NULL){
        if(temp -> id == -1){
            if(process -> required_size <= temp -> required_size){
                break;
            }
        }
        prev = temp;
        temp = temp -> next;
        count++;
    }
    //Verifica se o espaço enctrado é o suficiente
    if(temp -> required_size <= process -> required_size){
        printf("|ERRO - (Memoria Insuficiente)\n");
        return false;
    }
    //Faz a checagem se o ponteiro anterior é nulo ou a raiz
    if (prev != NULL){
        prev -> next = process;
    }
    else if(count == 0) {
        main_memory -> spaces = process;
    }

    //Faz a checagem se o processo vai utilizar todo o espaco 
    if(temp -> required_size == process -> required_size){
        next = temp -> next;
    }
    else {
        next = temp;
    }

    temp -> required_size -= process -> required_size;
    process -> init = temp -> init;
    process -> end = process -> init + process -> required_size;
    process -> next = next;

    main_memory -> free -= process -> required_size;
    main_memory -> space_count++;

    //Atualiza o inicio do espaco vazio
    if(temp -> required_size != 0){
        temp -> init = process -> end + 1;
    }
    else {
        free(temp);
    }
    printf("|Processo %d alocado\n|Tamanho: %d\n",process -> id,process -> required_size);
    printf("|Espaço restante: %d\n",main_memory -> free);
    return true;
}


int bf_allocation(struct memory *main_memory,struct space *process)
{
    int result = false;
    struct space *temp = main_memory -> spaces;
    struct space *temp_prev = NULL;

    int pos = 0;
    int bestfit = process -> required_size;
    int best_pos = 0;
    
    while(temp -> next != NULL){
        if(temp -> id == empty){ 
            if(((temp -> required_size) - (process -> required_size) < bestfit)
            && ((temp -> required_size) - (process -> required_size) >= 0)){
                bestfit = (temp -> required_size) - (process -> required_size);
                best_pos = pos;
            }
        }
        printf("pos %d ",best_pos);
        pos++;
        temp = temp -> next;
    }

    return result;
}

int compact(struct memory *main_memory)
{
    int result = false;
    if(main_memory -> spaces != NULL && main_memory -> space_count > 1){
        struct space *temp = main_memory -> spaces;
        struct space *prev = NULL;
        struct space *temp2 = temp -> next;
        while(temp != NULL){
            if(temp -> id == empty){
                while(temp2 -> next != NULL){
                    temp2 = temp2 -> next;
                }
                if(prev != NULL){
                    prev -> next = temp -> next;    
                }
                else{
                    main_memory -> spaces = temp -> next;
                }
                temp2 -> next = temp;
                temp -> next = NULL;
                // temp -> next = NULL;
            }
            prev = temp;
            temp = temp -> next;
        }

        temp = main_memory ->spaces;
        while(temp -> next != NULL){
            if(temp -> id == empty){
                if(temp -> next != NULL && temp -> next -> id == empty){
                    temp -> required_size += temp -> next -> required_size;
                    temp -> end = temp -> next -> end;
                    temp -> next -> next = NULL;
                    temp -> next -> id = to_remove;
                    temp -> next -> required_size = 0;
                    // temp -> next -> process = NULL;
                    // temp -> next = NULL;
                    free(temp -> next);
                    main_memory -> space_count--;
                }
            }
            temp = temp -> next;
        }
        
    }
    return result;
}

//Utils:-----------------------------------------------------------------------

char *getString()
{
    __fpurge(stdin);
    int len = 0;
    char actual_char;
    char *text = NULL;
    while(true){
        actual_char = getchar();
        if(actual_char == '\n'){
            break;
        }
        else {
            text = realloc(text,sizeof(char) * len + 1);
            text[len] = actual_char;
            len++;
        }
    }
    text = realloc(text,sizeof(char) * len);
    text[len] = '\0';
    return text;
}

void list_spaces(struct memory *main_memory)
{
    if(main_memory -> spaces != NULL){
        struct space *temp = main_memory -> spaces;
        int count = 0;
        while(temp != NULL && count < main_memory -> space_count){
            char title [] = "EMPTY SPACE";
            if(temp -> id == empty){
                printf("\n\n|EMPTY SPACE");
            }
            else if(temp -> id == to_remove){
                printf("\n\n|TO REMOVE");
            }
            else {
                printf("\n\n|ID: %d",temp -> id);
            }
            printf("\n|USED SPACE: %d\n|PROCESS: %s",temp -> required_size,temp -> process);
            temp = temp -> next;
            count++;
        }
    }
}

// struct space **map_space(struct memory *main_memory)
// {
//     struct space **result = NULL;
//     if(main_memory -> spaces != NULL){
//         printf("Ola");
//         struct space **map;
//         map = malloc(sizeof(*map) * (main_memory -> space_count));
        
//         struct space *temp = main_memory -> spaces;
//         int count = 0;
//         while(temp != NULL){
//             map[count] = temp;
//             temp = temp -> next;
//         }
//         result = map;
//     }
//     return result;
// }


int compareStrings(char *stringA,char *stringB){
    int lenA = strlen(stringA);
    int lenB = strlen(stringB);
    if (lenA != lenB){
        return false;
    }    
    for(int i = 0;i < lenA;i++){
        if(stringA[i] != stringB[i]){
            return false;
        }
    }
    return true;
}

int getCommandType(char *type)
{
    char *commands[] = {"FF","WF","BF","CF"};
    int result = -1;
    for(int i = 0; i< 4;i++){
        if(compareStrings(commands[i],type)){
            result = i;
            break;
        }
    }
    return result;
}

void setMainMemory(struct memory *main_memory){
    printf("\n|DIGITE O ESPACO TOTAL DA MEMORIA:\n");
    int size;
    int idGen = 0;
    scanf("%d",&size);
    main_memory = init_main_memory(size);
}

void getProcess(struct memory *main_memory)
{   
    printf("\n|DIGITE O ESPACO TOTAL DA MEMORIA:\n");
    int size;
    int idGen = 0;
    scanf("%d",&size);
    main_memory = init_main_memory(size);
    // main_memory -> si
    if(main_memory != NULL){
        
        char *command = getString();
        char *type;
        char *size;
        
        //Origem do split : https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
        char *delim = " ";
        char *ptr = strtok(command,delim);

        int count = 0;
        while(ptr != NULL)
        {
            switch(count){
                case 0 :
                    type = ptr;
                break;
                case 1:
                size = ptr;
                break;  
            }
            // printf("'%s'\n", ptr);
            ptr = strtok(NULL, delim);
            count++;
        }
        printf("|-\n|TYPE: %s\n|SIZE: %s",type,size);
        int req_size = atoi(size);
        // printf("%d",req_size);
        if(req_size > 0){
            struct space *new_space = create_space(idGen,command,req_size);
            switch(getCommandType(type)){
            case 0:
                ff_allocation(main_memory,new_space);
            break;
            case 1:
            break;
            case 2:
            break;
            case 3:
            break;
        }
        }

        // printf("\n%d",getCommandType(type));

        //Fim--------------------------------------------------------
        // printf("%s",ptr[0]);
    }

}

int main(void){
    struct memory *main_memory = init_main_memory(500);
    // struct memory *main_memory = NULL;
    // getProcess(main_memory);
    struct space *test = create_space(1,"FF 100",100);
    struct space *test2 = create_space(2,"FF 300",300);
    struct space *test3 = create_space(3,"BF 30",30);
    ff_allocation(main_memory,test);
    ff_allocation(main_memory,test2);
    // bf_allocation(main_memory,test3);
    printf("%d",main_memory -> size);

    list_spaces(main_memory);
    
    unllocate_space(main_memory,1);
    compact(main_memory);

    ff_allocation(main_memory,test3);
    list_spaces(main_memory);
    
    // char *text = getString();
    // printf("%s",text);
    // struct space **map = map_space(main_memory);

    // printf("%d",(int) *map[0] -> id);

    // free(test);
    // free(test2);
    // free(text);
    free(main_memory);
    return 0;
}