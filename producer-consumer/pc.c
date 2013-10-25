#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define COUNT 10
#define SIZE 12

sem_t buf_lock;		// controls access to the buffer
sem_t slot_avail;	// counts the slots available
sem_t item_avail;	// counts the items available
char buffer[COUNT][SIZE];
int in, out, count;


int main(int argc, char const *argv[])
{
	sem_init(&buf_lock, 0, 1); sem_init(&slot_avail, 0, 10);
	sem_init(&item_avail, 0, 0);



	return 0;
}


int producer()
// reads a string of the buffer slot size from the input file
// and places it into the next available buffer slot.
{

	return 0;
}

int consumer()
// Takes the next available string from a buffer slot and writes
// it into the output file, making the inputfile a copy of the
// outputfile
{

	return 0;
}