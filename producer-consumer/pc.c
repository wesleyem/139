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
pthread_t t[2];

int producer();
int consumer();

int main(int argc, char const *argv[])
{
	if (argc != 3) {
		// if the incorrect number of arguments are provided
		// let the user know and terminate
		printf("# of arguments:\t%d\n", argc);
		printf("%s\n", "Correct usage: $> ./a.out inputfile outputfile");
		return 0;
	} else {
		// Otherwise, proceed with opening the files for
		// reading and writing respectively
		FILE *inputfile, *outputfile;
		inputfile = fopen(argv[1], "r");
		outputfile = fopen(argv[2], "w");

		if ((inputfile == NULL) && (outputfile == NULL)) {
			// if the fopens fail for whatever reason let the
			// user know and terminate
			printf("%s\n", "Error opening files");
			return 0;
		} else {
			sem_init(&buf_lock, 0, 1); sem_init(&slot_avail, 0, 10);
			sem_init(&item_avail, 0, 0);
			printf("producer thread being created\n");
			pthread_create(&t[0], NULL, (void*) producer, (void*) 0);
			printf("consumer thread being created\n");
			pthread_create(&t[1], NULL, (void*) consumer, (void*) 1);

			pthread_join(t[0], NULL); pthread_join(t[1], NULL);
			pthread_exit(NULL);

		}



		fclose(inputfile); fclose(outputfile);
		return 0;
	}



	return 0;
}


int producer()
// reads a string of the buffer slot size from the input file
// and places it into the next available buffer slot.
{
	printf("Hello, I am a producer\n");
	return 0;
}

int consumer()
// Takes the next available string from a buffer slot and writes
// it into the output file, making the inputfile a copy of the
// outputfile
{
	printf("Hello, I am a consumer\n");
	return 0;
}