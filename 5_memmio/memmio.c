#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <signal.h> 
#include <string.h> 

struct stat buf;
char *mm_file;

int main(int argc, char *argv[])
{
	int ready = 0;
	FILE * fp;
	int fd, status;
	/*
		Makes sure filename argument is given, creates read/write permissions on file, and initializes file with string.
	*/
	if (argc == 2)
	{
		char *input = "memory mapping a disk file into virtual memory, done via the mmap call,\nallows file i/o to be treated as routine memory accesses.\nin this exercise, this file gets memory mapped first. Then two child\nprocesses: child-1 and child-2, each will make some changes to the file.\n";
		fp = fopen(argv[1], "w+b");
		fd = fileno(fp);
		fputs(input, fp);
		ready = 1;
	}
	else
	{
		printf("%s\n", "Incorrect usage, please supply filename.\n $> ./memmio filename");
	}

	if (ready)
	{
		pid_t pid[2];

		/*
			memory map file using mmap()
		*/
		if (fstat(fd, &buf) < 0)
		{
			perror("Error in fstat");
			return 1;
		}

		if ((mm_file = mmap((void *) 0, (size_t) buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t) 0)) == (caddr_t) - 1)
		{
			perror("Error in mmap");
			return 1;
		}

		/*
				Child process execution begins here with
				child number 1 and continues to child
				number 2
		*/
		if ((pid[1] = fork()) == 0)
		{
				printf("Child 1 %d reads: \n %s\n", getpid(), mm_file); 
				return 0;
		}
		if ((pid[2] = fork()) == 0)
		{

				return 0;
		}
		/* Parent waits for child processes to finish */
		waitpid(pid[1], &status, 0); waitpid(pid[2], &status, 0);
		printf("Exiting: %d\n", getpid());
		return 0;
	}
	return 0;
}

int replace_hyphen(char * str, char *orig, char *rep)
{
	static char buffer[4096];
	char *p;
	if(!(p = strstr(str, orig)))
    	return str;
     
    strncpy(buffer, str, p-str);
    buffer[p-str] = '\0';
     
    sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
     
    return buffer;
}
