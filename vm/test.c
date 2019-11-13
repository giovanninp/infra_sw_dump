#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pointer to array with binary number converted in convertBinary

typedef struct {
	int page;
	int offset;
}paging;

void intToBinary(int n, int *pointer);
paging translateAddress(int *pointer);
void searchAddress(paging address);
int twoToPow(int power);

int main(int argc, char const *argv[])
{
    int *binaryAddress;
    int logicalAddress;
    paging address;
    FILE *input;

    binaryAddress = malloc(32*sizeof(int));

    input = fopen(argv[1], "r");
    
    while(fscanf(input, "%d", &logicalAddress) > 0){
        intToBinary(logicalAddress, binaryAddress);
        address = translateAddress(binaryAddress);
        searchAddress(address);
    }
    free(binaryAddress);
    fclose(input);

    return 0;
}

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


void searchAddress(paging address)
{
    printf("Searching page %d with offset %d\n", address.page, address.offset);
}

