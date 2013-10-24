#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>

void *matrix(void *);
void *display(void *);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t t[5];
int C[4], A[8], B[8];


int main(int argc, char *argv[]) {
	if (argc == 17) {
		if (datavalidation(argc, argv)) {
			/*
				Data preprocessing. This is where the
				parameters are rolled into arrays for
				easier managment
			*/
			int i;
			for (i = 0; i < 8; i++) A[i] = atoi(argv[i+1]);
			for (i = 0; i < 8; i++) B[i] = atoi(argv[i+9]);
			/*
				Data preprocessing finished
			*/
			for (i = 0; i < 4; i++) {
				printf("Thread (%d) being created\n", i);
				pthread_create(&t[i], NULL, matrix, (void *) i);
			}
			printf("Thread (%d) being created\n", i);
			pthread_create(&t[4], NULL, display, (void *) i);
			printf("%s\n", "Waiting for Display thread to finish if it hasnt already...");
			pthread_join(t[4], NULL);
			pthread_exit(NULL);
		}
	} else {
		printf("%s\n", "Incorrect number of input or incorrect type of input");
		return 0;
	}
	return 0;
}
void *display(void *i) {
	int x = (int)i;
	int j;
	printf("Thread ID (%u)\n", pthread_self());
	printf("%s\n", "Waiting for matrix calculation threads to finish if they havent already...");
	for (j = 0; j < 4; j++) pthread_join(t[j], NULL);
	//printf("%s\t%d\n", "display thread", A[0]);
	//while ((C[0] == 0) && (C[1] == 0) && (C[2] == 0) && (C[3] == 0)) {}
	pthread_mutex_lock(&mutex);
	printf("The first row the resulting matrix is %d %d\n", C[0], C[1]);
	printf("The second row of the resulting matrix is %d %d\n", C[2], C[3]);
	pthread_mutex_unlock(&mutex);
	printf("%s%d%s\n", "Thread ", x, " exiting");
	pthread_exit(NULL);
}
void *matrix(void *num) {
	int x = (int)num;
	printf("Thread ID (%u)\n", pthread_self());
	pthread_mutex_lock(&mutex);
	switch (x) {
		case 0: C[0] = (A[0]*B[0]) + (A[1]*B[2]) + (A[2]*B[4]) + (A[3]*B[6]); printf("C[0]=%d\n", C[0]); break;
		case 1: C[1] = (A[0]*B[1]) + (A[1]*B[3]) + (A[2]*B[5]) + (A[3]*B[7]); printf("C[1]=%d\n", C[1]); break;
		case 2: C[2] = (A[4]*B[0]) + (A[5]*B[2]) + (A[6]*B[4]) + (A[7]*B[6]); printf("C[2]=%d\n", C[2]); break;
		case 3: C[3] = (A[4]*B[1]) + (A[5]*B[3]) + (A[6]*B[5]) + (A[7]*B[7]); printf("C[3]=%d\n", C[3]); break;
	}
	pthread_mutex_unlock(&mutex);
	//printf("%d\n", C[3]);
	printf("%s%d%s\n", "Thread ", x, " exiting");
	pthread_exit(NULL);
}
/**********************************************************
Parameter(s): 	char array of arrays
Return type: 	int
Description:	Runs each argument through the isvalid
				method to determine if the data meets
				the necessary criteria

**********************************************************/
int datavalidation(int c, char *a[]) {
	int i;
	for (i = 1; i < c; i++) { if (isvalid(a[i]) == 0) return 0; }
	return 1;
}
/**********************************************************
Parameter(s): 	char array
Return type: 	int
Description:	Returns 0 when string supplied does not meet
				[+|-](\digit) where [] signify optional and
				() required.
				
***********************************************************/
int isvalid(char a[]) {
	int i;
	if ((a[0] == '+') || (a[0] == '-')) {
		/* char is  + or - */
		i = 1;
		while (a[i] != '\0') {
			if (isdigit(a[i]) != 0) { /* char is digit */ } 
			else {
				/* char is not digit */
				printf("%s\n", "Incorrect type of input\nError code: 1A");
				return 0;
			}
			i++;
		}
	} else {
		i = 0;
		while (a[i] != '\0') {
			if (isdigit(a[i]) != 0) { /* char is digit */ }
			else {
				/* char is not digit */
				printf("%s\n", "Incorrect type of input\nError code: 1B");
				return 0;
			}
			i++;
		}
	}
	return 1;
}
