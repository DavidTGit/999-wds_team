#include "processParrent.h"


int main()
{
	SPI_DEVICE spi_dev[max_devices];
    pid_t pid, pidOP, pidP, pidC, process_id;
    pidOP = getpid();
	printf("This is the original parrent process, pid=%d\n", pidOP);


	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0)
	{
		printf("There is a error after fork, pid=%d\n", process_id);

		// Return failure in exit status
		exit(1);
	}
	// PARENT PROCESS. Need to kill it.
	else if (process_id > 0)
	{
		pidP = getpid();
		printf("This is the parrent process after fork, pid=%d and getpid=%d \n", process_id, pidP);

		// return success in exit status
		exit(0);
	}
	else//child process
	{
		pidC = getpid();
        printf("This is the child process, pid=%d and getpid=%d \n", process_id, pidC);

    }

	
    return 0;

}
