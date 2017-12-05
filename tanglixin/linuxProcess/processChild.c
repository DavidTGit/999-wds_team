#include "processParrent.h"

int main()
{

    pid_t pid;
    printf("This is the child process.\n");
    system ("ps -a | grep processchd ");
    pid = getpid();
    printf("this child process pid =%d kill the number if you like\n", pid);
    while(1)
    {
    	int i,j;
    	for(i = 1; i <= 10; i++)
    	{
    		for(j = 0; j < i; j++)
    		{
        		printf("* ");
    		}
			printf("\n");
        	sleep(2);
    	}
		printf("this child process pid =%d kill the number if you like\n", pid);

    }
    return 0;
}
