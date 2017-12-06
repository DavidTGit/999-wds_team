#include "processParrent.h"

int main(){
	FILE* fp;
	pid_t pid;
	int i,j;
	char filename[] = "/tmp/outchd.txt";

    printf("This is the child process gggggggggg\n");
    system ("ps -a | grep processchd ");
	umask(0);
	pid = setsid();
	if(pid<0){
    printf("This is the child process failed failed failed\n");
	exit(0);
	}

	chdir("/");
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
    pid = getpid();

	fp = fopen(filename, "a+");
	if(fp == NULL)
	{
		fclose(fp);
		printf("open outchd.txt failed! This is the child process. pid = %d\n",pid);
		return -1;
	}
	system("chmod 666 /tmp/outchd.txt");
    fprintf(fp, "this child process pid =%d kill the number if you like\n", pid);
    while(child_counter--)
    {
    	for(i = 1; i <= 10; i++)
    	{
    		for(j = 0; j < i; j++)
    		{
        		fprintf(fp, "* ");
    		}
			fprintf(fp, "\n");
        	sleep(2);
    	}
		fprintf(fp, "this child process pid =%d kill the number if you like\n", pid);

    }
	if(fp != NULL)
		fclose(fp);
	return 0;
}

