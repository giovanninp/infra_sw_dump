virtual_mainmory: tlb.o input_read.o LRU.o virtual_mainmory.o
	gcc tlb.o -lpthread input_read.o LRU.o virtual_mainmory.o -o virtual_mainmory

tlb.o: tlb.c
	gcc -c tlb.c 

input_read.o: input_read.c
	gcc -c input_read.c

LRU.o: LRU.c
	gcc -c LRU.c

virtual_mainmory.o: virtual_mainmory.c
	gcc -c virtual_mainmory.c

clean:
	rm * .o virtual_mainmory