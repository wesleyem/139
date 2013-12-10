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

/*
 *     int filedesc = open("testfile.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR | S_IXGRP | S_IXOTH);
 *         if(filedesc < 0)
 *                 return 1;
 *                  
 *                      if(write(filedesc,"This will be output to testfile.txt\n", 36) != 36)
 *                          {
 *                                  write(2,"There was an error writing to testfile.txt\n",43);
 *                                          return 1;
 *                                              }
 *                                                  return 0;
 *
 *                                                  */

	int ready = 0;
	int fd, status, ret;
	FILE * fp;
	/*
 * 		Makes sure filename argument is given, creates read/write permissions on file, and initializes file with string.
 * 			*/
	if (argc == 2)
	{
		char *input = "memory mapping a disk file into virtual memory, done via the mmap call,\nallows file i/o to be treated as routine memory accesses.\nin this exercise, this file gets memory mapped first. Then two child\nprocesses: child-1 and child-2, each will make some changes to the file.\n";
		fd = open(argv[1], O_CREAT | O_RDWR, (mode_t) S_IRWXU); 
		printf("%d\n", sizeof(input));
		if (write(fd, input, 512) != 512)
		{
			char * t = "There was an error writing to the file\n";
			write(2, t, sizeof(t));
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

		/*
 * 			memory map file using mmap()
 * 					*/
		if ((ret = stat(argv[1], &buf)) < 0)
		{
			perror("Error in stat");
			return 1;
		}

		if ((mm_file = mmap(0, (size_t) buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void*) -1)
		{
			printf("%d\n", mm_file);
			perror("Error in mmap");
			return 1;
		}

		/*
 * 				Child process execution begins here with
 * 								child number 1 and continues to child
 * 												number 2
 * 														*/
		if ((pid[0] = fork()) == 0)
		{
				printf("Child 1 %d reads: \n %s\n", getpid(), mm_file); 
				return 0;
		}
		if ((pid[1] = fork()) == 0)
		{

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
