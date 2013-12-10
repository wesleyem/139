#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <signal.h> 
#include <string.h> 
#include <errno.h>

struct stat buf;
char *mm_file;

int main(int argc, char *argv[])
{
	int ready = 0;
	int fd, status, ret, len;
	FILE * fp;
	char * find_ptr;
	char temp[512];
	/*
 * 		Makes sure filename argument is given, creates read/write permissions on file, and initializes file with string.
 * 			*/
	if (argc == 2)
	{
		char *input = "memory mapping a disk file into virtual memory, done via the mmap call,\nallows file i/o to be treated as routine memory accesses.\nin this exercise, this file gets memory mapped first. Then two child\nprocesses: child-1 and child-2, each will make some changes to the file.\n";
		len = strlen(input);
		fd = open(argv[1], O_CREAT | O_RDWR, (mode_t) S_IRWXU); 
		if (write(fd, input, len) != len)
		{
			char * t = "There was an error writing to the file\n";
			write(2, t, strlen(t));
			return 1;
		}
		ready = 1;
	}
	else
	{
		ready = 0;
		printf("%s\n", "Incorrect usage, please supply filename.\n $> ./memmio filename");
	}

	if (ready)
	{
		pid_t pid[2];

		/* fstat, get stats about the file */
		if ((ret = fstat(fd, &buf)) < 0)
		{
			perror("Error in fstat");
			return 1;
		}

		if ((mm_file = mmap(0, (size_t) buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void*) -1)
		{
			printf("%d\n", mm_file);
			perror("Error in mmap");
			return 1;
		}

		/*
			Child process execution
 		*/
		if ((pid[0] = fork()) == 0)
		{
			printf("Child 1 %d reads: \n%s\n", getpid(), mm_file);
			strncpy(temp, mm_file, len);

			int i;
			for (i = 0; i < len; i++)
			{
				if ((int)temp[i] >= 97 && (int)temp[i] <= 122)
				{
					temp[i] = (char)(((int)temp[i]) - 32);	
				}
			}
			strncpy(mm_file, temp, len);
			msync(0, (size_t) buf.st_size, MS_SYNC);
			printf("Child 1 %d reads again: \n%s\n", getpid(), mm_file); 

			return 0;
		}
		if ((pid[1] = fork()) == 0)
		{
			sleep(1);
			printf("Child 2 %d reads: \n%s\n", getpid(), mm_file);
			strncpy(temp, mm_file, len);
			if ((find_ptr = strstr(temp, "MEMORY MAPPED")) == NULL)
			{
				perror("find_ptr is null");
			}
			strncpy(find_ptr, "MEMORY-MAPPED", strlen("MEMORY-MAPPED"));
			if ((find_ptr = strstr(temp, "CHANGES")) == NULL)
			{
				perror("find_ptr is null1");	
			}
			strncpy(find_ptr, "UPDATES", strlen("UPDATES"));
			strncpy(mm_file, temp, len);
			printf("Child 2 %d reads again: \n%s\n", getpid(), mm_file);
			return 0;
		}
		/* Parent waits for child processes to finish */
		waitpid(pid[1], &status, 0); waitpid(pid[2], &status, 0);
		printf("Exiting: %d\n", getpid());
		close(fd);
		return 0;
	}
	return 0;
}