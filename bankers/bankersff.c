#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implementação do algoritimo como sitado no livro do Dinossauro*/
// Need[i] <= available --> available = available + alocation 
// saida[i] = True

#define true 1
#define false 0

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURSES 4
//O Exemplo do livro diz 4 

int available[NUMBER_OF_RESOURSES];
//Total de processos - Total de processos alocados

int total[NUMBER_OF_RESOURSES] = {10,5,7};

//Caso de teste. A entrega deverá ler de uma arquivo
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURSES] = {{6,4,7,3},
														 {4,2,3,2},
														 {2,5,3,3},
														 {6,3,3,2},
														 {5,6,7,5}};

int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURSES];
  
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURSES];

void bankers();
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);

int commander_receiver(){
	__fpurge(stdin);
	char actual_char;
	int actual_pos = 0;
	int actual_len = 1;
	char *text = (char *)malloc(sizeof(char)*actual_len);
	while(true){
		actual_char = getchar();
		if (actual_char == '\n')break;
		actual_len++;
		text = (char)realloc(text, sizeof(char) * actual_len);
		text[actual_pos] = actual_char;
		actual_pos++;
	}
}

int main(int argc, char const *argv[]){
	if(argc - 1 != NUMBER_OF_RESOURSES){
		printf("Error: To many or to few inputs\n");
		exit(1);
	}
	for (int i = 1; i < argc; i++){
		available[i - 1] = atoi(argv[i]);// Inicializa Available
	}

	return 0;
}

int request_resources(int customer_num, int request[]){
	for (int i = 0; i < NUMBER_OF_RESOURSES; i++){
		need[customer_num][i] = request[i];
	}
	int unsafe = 0;
	for (int i = 0; i < NUMBER_OF_RESOURSES; i++){
		if(available[i] < need[customer_num][i]){ // Lógica pricipal !(Need <= Available)
			unsafe = 1;
			break;
		}
	}
	if(unsafe){
		return -1;
	}else{
		for (int i = 0; i < NUMBER_OF_RESOURSES; i++){// Alocando recursos pedidos
			allocation[customer_num][i] = request[i];
		}
		return 0;
	}
			
}

void release_resources(int customer_num, int release[]){
	for (int i = 0; i < NUMBER_OF_RESOURSES; i++){
		available[i] += release[i];
		if(allocation[customer_num][i] != 0){
			allocation[customer_num][i] -= release[i];
		}
	}
}

void bankers(){// O projeto requer adaptar esse codigo.
	int saida[NUMBER_OF_CUSTOMERS];
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		saida[i] = 0;
	}

	printf("Need Matrix\n");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		for (int j = 0; j < NUMBER_OF_RESOURSES; j++){
			need[i][j] = maximum[i][j] - allocation[i][j];
			printf("%d ",need[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	int safe_order[NUMBER_OF_CUSTOMERS];
	int order = 0;
	for (int n = 0; n < NUMBER_OF_CUSTOMERS; n++){ // O Maximo de tentativas O(O pior) Será tentar o numero de processos x numero de processos!
		for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
			if (saida[i] == 0){
				int unsafe = 0;
				for (int j = 0; j < NUMBER_OF_RESOURSES; j++){
					if(available[j] < need[i][j]){ // Lógica pricipal !(Need <= Available)
						unsafe = 1;
						break;
					}
				}
				if(!unsafe){
					saida[i] = 1;
					safe_order[order] = i;
					order++;
					for (int j = 0; j < NUMBER_OF_RESOURSES; j++){
						available[j] += allocation[i][j];
					}
				}
			}
		}
	}

	for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		printf("Process %d\n", safe_order[i]);
	}
	

}

int init_max_array(){
	//Abrir arquivo e inicialiazar a Matrix Maximum. 
	return 1;
}

