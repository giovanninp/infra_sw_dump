#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implementação do algoritimo como sitado no livro do Dinossauro*/
// Need[i] <= available --> available = available + alocation 
// saida[i] = True

#define NUMBER_OF_CUSTUMERS 5
#define NUMBER_OF_RESOURSES 3
//O Exemplo do livro diz 4 

int available[NUMBER_OF_RESOURSES] = {3,3,2};
//Total de processos - Total de processos alocados

int total[NUMBER_OF_RESOURSES] = {10,5,7};

int maximum[NUMBER_OF_CUSTUMERS][NUMBER_OF_RESOURSES] = {{7,5,3},
														 {3,2,2},
														 {4,0,2},
														 {4,2,2},
														 {5,3,3}};
//Caso de teste. A entrega deverá ler de uma arquivo
int allocation[NUMBER_OF_CUSTUMERS][NUMBER_OF_RESOURSES] = {{0,1,0},
															{2,0,0},
															{3,0,2},
															{2,1,1},
															{0,0,2}};
int need[NUMBER_OF_CUSTUMERS][NUMBER_OF_RESOURSES];

void bankers();

int main(int argc, char const *argv[]){
	bankers();
	return 0;
}

void bankers(){
	int saida[NUMBER_OF_CUSTUMERS];
	for (int i = 0; i < NUMBER_OF_CUSTUMERS; i++){
		saida[i] = 0;
	}

	printf("Need Matrix\n");
	for (int i = 0; i < NUMBER_OF_CUSTUMERS; i++){
		for (int j = 0; j < NUMBER_OF_RESOURSES; j++){
			need[i][j] = maximum[i][j] - allocation[i][j];
			printf("%d ",need[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	int safe_order[NUMBER_OF_CUSTUMERS];
	int order = 0;
	for (int n = 0; n < NUMBER_OF_CUSTUMERS; n++){ // O Maximo de tentativas O(O pior) Será tentar o numero de processos x numero de processos!
		for (int i = 0; i < NUMBER_OF_CUSTUMERS; i++){
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

	for (int i = 0; i < NUMBER_OF_CUSTUMERS; i++){
		printf("Process %d\n", safe_order[i]);
	}
	

}

