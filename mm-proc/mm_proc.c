#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>


int main(int argc, char *argv[]) {
	if (argc == 17) {
		if (datavalidation(argc, argv)) {
			/*
				Data preprocessing. This is where the
				parameters are rolled into arrays for
				easier managment
			*/
			char string[10], readbuffer[4][80];
			int A[8], B[8], i, status, mypipe[4][2], x, nbytes;
			pid_t pid[5];

			for (i = 0; i < 8; i++) { A[i] = atoi(argv[i+1]); }
			for (i = 0; i < 8; i++) { B[i] = atoi(argv[i+9]); }
			/*
				Data preprocessing finished
			*/
			/*
				Initialize 4 pipes
			*/
			if (pipe(mypipe[0])) printf("%s\n", "mypipe[0] failed");
			if (pipe(mypipe[1])) printf("%s\n", "mypipe[1] failed");
			if (pipe(mypipe[2])) printf("%s\n", "mypipe[2] failed");
			if (pipe(mypipe[3])) printf("%s\n", "mypipe[3] failed");
			/*
				Child process execution begins here with
				child number 1 and continues to child 
				number 5
			*/
			if ((pid[1] = fork()) == 0) {
				printpids();
				close(mypipe[0][0]); /* close read */
				x = (A[0]*B[0]) + (A[1]*B[2]) + (A[2]*B[4]) + (A[3]*B[6]);
				sprintf(string, "%d", x);
				write(mypipe[0][1], string, (strlen(string)+1));
				printf("%s %d\n", "Exiting:", getpid());
				close(mypipe[0][1]);
				close(mypipe[1][1]); close(mypipe[2][1]); close(mypipe[3][1]);
				close(mypipe[1][0]); close(mypipe[2][0]); close(mypipe[3][0]);
				return 0;
			}
			if ((pid[2] = fork()) == 0) {
				printpids();
				close(mypipe[1][0]); /* close read */
				x = (A[0]*B[1]) + (A[1]*B[3]) + (A[2]*B[5]) + (A[3]*B[7]);
				sprintf(string, "%d", x);
				write(mypipe[1][1], string, (strlen(string)+1));	
				printf("%s %d\n", "Exiting:", getpid());			
				close(mypipe[1][1]);
				close(mypipe[0][1]); close(mypipe[2][1]); close(mypipe[3][1]);
				close(mypipe[0][0]); close(mypipe[2][0]); close(mypipe[3][0]);
				return 0;
			}
			if ((pid[3] = fork()) == 0) {
				printpids();
				close(mypipe[2][0]); /* close read */
				x = (A[4]*B[0]) + (A[5]*B[2]) + (A[6]*B[4]) + (A[7]*B[6]);
				sprintf(string, "%d", x);
				write(mypipe[2][1], string, (strlen(string)+1));
				printf("%s %d\n", "Exiting:", getpid());
				close(mypipe[2][1]);
				close(mypipe[0][1]); close(mypipe[1][1]); close(mypipe[3][1]);
				close(mypipe[0][0]); close(mypipe[1][0]); close(mypipe[3][0]);
				return 0;
			}
			if ((pid[4] = fork()) == 0) {
				printpids();
				close(mypipe[3][0]); /* close read */
				x = (A[4]*B[1]) + (A[5]*B[3]) + (A[6]*B[5]) + (A[7]*B[7]);
				sprintf(string, "%d", x);
				write(mypipe[3][1], string, (strlen(string)+1));	
				printf("%s %d\n", "Exiting:", getpid());			
				close(mypipe[3][1]);
				close(mypipe[0][1]); close(mypipe[1][1]); close(mypipe[2][1]);
				close(mypipe[0][0]); close(mypipe[1][0]); close(mypipe[2][0]);
				return 0;
			}
			if ((pid[5] = fork()) == 0) {
				printpids();
				/* close write ends of pipes */
				close(mypipe[0][1]); close(mypipe[1][1]); close(mypipe[2][1]); close(mypipe[3][1]);
				nbytes = read(mypipe[0][0], readbuffer[0], sizeof(readbuffer[0]));
				nbytes = read(mypipe[1][0], readbuffer[1], sizeof(readbuffer[1]));
				printf("The first row the resulting matrix is %s %s\n", readbuffer[0], readbuffer[1]);
				nbytes = read(mypipe[2][0], readbuffer[2], sizeof(readbuffer[2]));
				nbytes = read(mypipe[3][0], readbuffer[3], sizeof(readbuffer[3]));
				printf("The second row of the resulting matrix is %s %s\n", readbuffer[2], readbuffer[3]);
				printf("%s\n", "Display exiting");
				close(mypipe[0][0]); close(mypipe[1][0]); close(mypipe[2][0]); close(mypipe[3][0]);
				return 0;
			}
			/* Parent waits for child processes to finish */
			waitpid(pid[1], &status, 0); waitpid(pid[2], &status, 0);
			waitpid(pid[3], &status, 0); waitpid(pid[4], &status, 0);
			waitpid(pid[5], &status, 0);
			printf("Exiting: %d\n", getpid());
			return 0;
		}
	} else {
		printf("%s\n", "Incorrect number of input or incorrect type of input");
		return 0;
	}
	return 0;
}
/**********************************************************
Parameter(s): 	None
Return type: 	int
Description:	Prints the child and parent pids

**********************************************************/
int printpids() {
	printf("Child pid: %d\n", getpid());
	printf("Parent pid: %d\n", getppid());
	return 0;
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
		if (isdigit(a[1])) {
			while (a[i] != '\0') {
				if (isdigit(a[i]) != 0) { /* char is digit */ } 
				else {
					/* char is not digit */
					printf("%s\n", "Incorrect type of input\nError code: 1A");
					return 0;
				}
				i++;
			}
		} else { printf("%s\n", "Incorrect type of input\nError code: 1A"); return 0; }
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