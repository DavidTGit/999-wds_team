#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <linux/input.h>

int main(int argc, char * const argv[])
{    
	struct input_event event;  
    int fd_mouse = 0;
    int fd_led = 0; 
    int l_val=0;//on == 1, off == 0;
    int s_val=0;
    int e_val=0;    
    int ret = 0;
    
    fd_mouse = open(argv[1],O_RDONLY);
    if (fd_mouse < 0)
	{
		printf("can't open mouse!\n");
	}
	
    fd_led = open("/dev/led",O_RDWR);
    if (fd_led < 0)
	{
		printf("can't open led device!\n");
	}
    while(1){
        ret = read(fd,&event,sizeof(event));
//        sleep(1);
	switch(event.code){
		case KEY_L:
			printf("key L clicked, inverse led 1\n");
			
			break;
		case KEY_S:
			printf("key S clicked, inverse led 3\n");
			break;
		case KEY_ENTER:
			printf("key enter clicked\n");
			break;
		default:
			break;
		}
    }
   return 0;
}
