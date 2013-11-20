#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>
#define COUNT 10
#define SIZE 12

pthread_cond_t empty_slot = PTHREAD_COND_INITIALIZER;
pthread_cond_t avail_item = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buflock = PTHREAD_MUTEX_INITIALIZER;

char buffer[COUNT][SIZE];
int in = 0, out = 0, count = 0;
FILE *inputfile, *outputfile;
pthread_t t[2];

void* producer();
void* consumer();

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
		inputfile = fopen(argv[1], "rb");
		outputfile = fopen(argv[2], "wb");
		if ((inputfile == NULL) && (outputfile == NULL)) {
			// if the fopens fail for whatever reason let the
			// user know and terminate
			printf("%s\n", "Error opening files");
			return 0;
		} else {
			pthread_create(&t[0], NULL, producer, (void*) 0);
			pthread_create(&t[1], NULL, consumer, (void*) 1);
			pthread_join(t[0], NULL); pthread_join(t[1], NULL);
			printf("Main thread finished\n");
			fclose(inputfile); fclose(outputfile);
			pthread_exit(NULL);
		}
	}
}

void* producer()
// reads a string of the buffer slot size from the input file
// and places it into the next available buffer slot.
{
	char tempbuff[SIZE];
	strncpy(tempbuff, "TempBuffer", sizeof(tempbuff));
	printf("Hello, I am a producer\n");
	while(1) {
		
		if (fgets(tempbuff, SIZE, inputfile) != NULL) {
			// producer has created an item and would like to
			// place it into the buffer
			pthread_mutex_lock(&buflock);
			if(count == COUNT) {
				pthread_cond_wait(&empty_slot, &buflock);
			}
			strncpy(buffer[in], tempbuff, SIZE);
			// 12 Bytes have been placed into the buffer by the
			// producer.
			//printf("%s\ncount=%d\n", buffer[in], count);
			// count is incremented after item is placed into the
			// buffer so it represents the true count of items in
			// the buffer.
			count++;
			// in=(in+1)%COUNT so we use buffer as a circular
			// array.
			in = (in + 1) % COUNT;
			pthread_cond_signal(&avail_item);
			pthread_mutex_unlock(&buflock);
		} else {
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}

void* consumer()
// Takes the next available string from a buffer slot and writes
// it into the output file, making the outputfile a copy of the
// inputfile
{
	char tempbuff[SIZE];
	strncpy(tempbuff, "TempBuffer", sizeof(tempbuff));
	printf("Hello, I am a consumer\n");
	while (1) {
		pthread_mutex_lock(&buflock);
		if(count == 0) {
			pthread_cond_wait(&avail_item, &buflock);
		}
		strncpy(tempbuff, buffer[out], sizeof(tempbuff));
		out = (out + 1) % COUNT;
		count--;
		pthread_cond_signal(&empty_slot);
		pthread_mutex_unlock(&buflock);
		fputs(tempbuff, outputfile);
		//printf("%s\ncount=%d\n", tempbuff, count);
		// if the eof has been hit on inputfile and there is
		// is nothing left in the buffer, then consumer has
		// nothing left to consume and must terminate
		if ((feof(inputfile) != 0) && (count == 0)) {
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}