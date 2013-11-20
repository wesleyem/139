# OS Principles

This repo contains various assignments I am working on for my Computer Operating Systems class

###### Development environment
	$> gcc --version
	gcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3

AND

	$> gcc --version
	gcc (GCC) 4.4.7 20120313 (Red Hat 4.4.7-3)

### Assignments


#### [1] Matrix Multiplication - Multiprocessing
	$> gcc mm_proc.c
	$> ./a.out 1 2 3 4 1 2 3 4 1 3 2 4 1 3 2 4
	Child pid: 24129
	Child pid: 24130
	Parent pid: 24125
	Parent pid: 24125
	Exiting: 24129
	Child pid: 24128
	Parent pid: 24125
	Exiting: 24128
	Child pid: 24127
	Parent pid: 24125
	Exiting: 24127
	Child pid: 24126
	Parent pid: 24125
	The first row the resulting matrix is 16 36
	The second row of the resulting matrix is 16 36
	Display exiting
	Exiting: 24126
	Exiting: 24125

#### [2] Matrix Multiplication - Multithreading
	$> gcc mm_threads.c -lpthread
	$> ./a.out 1 2 3 4 1 2 3 4 1 3 2 4 1 3 2 4
	Thread (0) being created
	Thread (1) being created
	Thread (2) being created
	Thread (3) being created
	Thread (4) being created
	Waiting for Display thread to finish if it hasnt already...
	Thread ID (1227265792)
	Waiting for matrix calculation threads to finish if they havent already...
	Thread ID (1235658496)
	C[3]=36
	Thread 3 exiting
	Thread ID (1244051200)
	C[2]=16
	Thread 2 exiting
	Thread ID (1252443904)
	C[1]=36
	Thread 1 exiting
	Thread ID (1260836608)
	C[0]=16
	Thread 0 exiting
	The first row the resulting matrix is 16 36
	The second row of the resulting matrix is 16 36
	Thread 4 exiting

#### [3] Bounded-Buffer Producer/Consumer Problem - Multithreading
###### Semaphores

Useful resource: http://www.csee.wvu.edu/~jdm/classes/cs550/notes/tech/mutex/pc-sem.html

	$> gcc pc.c -lpthread -Lposix4
	$> ./a.out input output
	Hello, I am a producer
	Hello, I am a consumer
	Main thread finished
	$> cat output
	The bounded-buffer producer/consumer approach can be applied to
	a variety of different scenarios. In this programming assignment,
	you will use the bounded-buffer producer/consumer approach to
	implement a file copying operation. Both a producer thread and
	a consumer thread work with a buffer of 10 slots. The producer
	reads a string of 12-byte at a time from the input file (i.e.,
	this file), and writes it to the next available slot in the
	buffer. The consumer, on the other hand, reads a 12-byte string
	from the next filled buffer slot, and writes it to the output
	file such that the output file becomes a verbatim copy of the
	input file. Note that the last string may not be exactly 12-byte
	long. Semaphores are used to enforce the mutually exclusive
	access to the buffer and the synchronization conditions between
	the producer and the consumer.
	$> ./a.out input
	# of arguments: 2
	Correct usage: $> ./a.out inputfile outputfile

#### [4] Bounded-Buffer Producer/Consumer Problem - Multithreading
###### Mutex locks and condition variables

	$> gcc pc-mutex.c -o pc-mutex -lpthread
	$> ./pc-mutex input output
	Hello, I am a consumer
	Hello, I am a producer
	Main thread finished
	$> diff input output
	$> cat output
	The synchronization mechanism of mutex-lock and condition-variables plays
	a pivotal role in monitors. This programming assignment gives you an
	opportunity to use the mechanism in the bounded-buffer producer-consumer
	implementation of a file copying operation. Unlike in Program 3 where we
	need to make sure that the order of two semaphore wait operations is
	correct, here when a thread has to block, the pthread_cond_wait will
	automatically release the mutex lock, hence avoiding potential deadlock.
	Many of the other functionalities in your program 3 can still be reused.
	This exercise will shed light on the difference between the semaphore based
	approach and the mutex-lock/condition-variable based approach.

