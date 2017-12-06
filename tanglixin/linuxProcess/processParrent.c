#include "processParrent.h"

int funprt(){
    pid_t pid;
	int i,j;

    printf("This is the parrent process.\n");
    system ("ps -a | grep processmain ");
    pid = getpid();
    printf("this parrent process pid =%d kill the number if you like\n", pid);
    while(main_counter--)
    {
    	for(i = 10; i >= 1; i--)
    	{
    		for(j = 0; j < i; j++)
    		{
        		printf("* ");
    		}
			printf("\n");
        	sleep(2);
    	}
		printf("this parrent process pid =%d kill the number if you like\n", pid);

    }
    return 0;
}
int funchd()
{
	FILE* fp;
	pid_t pid;
	int i,j;
	char filename[] = "/tmp/outchd.txt";

    printf("This is the child process..........\n");
    system ("ps -a | grep processchd ");
    pid = getpid();

	fp = fopen("/tmp/outchd.txt", "a+");
	if(fp == NULL)
	{
		fclose(fp);
		printf("open outchd.txt failed! This is the child process. pid = %d\n",pid);
		return -1;
	}
	system("chmod 666 /tmp/outchd.txt");
    printf("this child process pid =%d kill the number if you like\n", pid);
    while(child_counter--)
    {
    	for(i = 1; i <= 10; i++)
    	{
    		for(j = 0; j < i; j++)
    		{
        		printf("$ ");
    		}
			printf("\n");
        	sleep(2);
    	}
		printf("this child process pid =%d kill the number if you like\n", pid);

    }
	if(fp != NULL)
		fclose(fp);
	return 0;

}
#if 0
int main()
{
	FILE* fp;
    char filemain[] = "/tmp/outmain.txt";
    //SPI_DEVICE spi_dev[max_devices];
    pid_t pid, pidOP, pidP, pidC, process_id;
    int ret;
    pidOP = getpid();
    printf("This is the original parrent process, pid=%d\n", pidOP);
    fp = fopen(filemain,"a+");
	if(fp == NULL)
	{
		fclose(fp);
		printf("This is the parrent process. pid = %d\n",pidOP);
		return -1;
	}

    // Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0)
    {
        printf("There is a error after fork, pid=%d\n", process_id);

        // Return failure in exit status
        exit(1);
    }
    // PARENT PROCESS.
    else if (process_id > 0)
    {
        pidP = getpid();
        printf("This is the parrent process after fork, pid=%d and getpid=%d \n", process_id, pidP);
        fprintf(fp, "This is the parrent process after fork, pid=%d and getpid=%d \n", process_id, pidP);
        funprt();
        exit(0);
    }
    else//child process
    {
        pidC = getpid();
        printf("This is the child process, pid=%d and getpid=%d \n", process_id, pidC);
        fprintf(fp, "This is the child process, pid=%d and getpid=%d \n", process_id, pidC);

		//system("/usr/bin/processchd");
		
		ret = execl("/usr/bin/processchd","/usr/bin/processchd",(char *)NULL);
        if(ret == -1)
        {
            fprintf(fp, "child exec func return error = %s\n", strerror(errno));
            exit(0);
        }
        
        exit(0);
    }

    return 0;
}
#else
int main()
{
    //I_DEVICE spi_dev[max_devices];
    pid_t pid, pidOP, pidP, pidC, process_id;
    int ret;

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
    // PARENT PROCESS.
    else if (process_id > 0)
    {
        pidP = getpid();
        printf("This is the parrent process after fork, pid=%d and getpid=%d \n", process_id, pidP);
        funprt();
        exit(0);
    }
    else//child process
    {
        pidC = getpid();
        printf("This is the child process, pid=%d and getpid=%d \n", process_id, pidC);
		funchd();
		//system("/usr/bin/processchd");
        exit(0);
    }

    return 0;
}

#endif
