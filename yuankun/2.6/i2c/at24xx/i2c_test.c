
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* i2c_test r addr
 * i2c_test w addr val
 */

void print_usage(char *file)
{
	printf("%s r addr\n", file);
	printf("%s w addr val\n", file);
}

#define MAX_LEN	128

int main(int argc, char **argv)
{
	int fd;
	unsigned char buf[128];
	int ret, wr_len=0;
	
	if ((argc != 3) && (argc != 4))
	{
		print_usage(argv[0]);
		return -1;
	}

	fd = open("/dev/at24xx", O_RDWR);
	if (fd < 0)
	{
		printf("can't open /dev/at24xx\n");
		return -1;
	}

	if (strcmp(argv[1], "r") == 0)
	{
		buf[0] = strtoul(argv[2], NULL, 0); //get offset position
//		read(fd, buf, 1);
//		read(fd, buf, atoi(argv[3]));
		read(fd, buf, strtoul(argv[3], NULL, 0));	
		//get argv[3] size of data
//		printf("data: %c, %d, 0x%2x\n", buf[0], buf[0], buf[0]);
		printf("data is : %s\n",buf);
	}
	else if (strcmp(argv[1], "w") == 0)
	{
//		buf[0] = strtoul(argv[2], NULL, 0);
//		buf[1] = strtoul(argv[3], NULL, 0);
//		write(fd, buf, 2);
		wr_len = strlen(argv[3]);
//		printf("%s\n",argv[3]);
		wr_len = (wr_len>MAX_LEN)? MAX_LEN:wr_len;
		strncpy(buf,argv[3],wr_len);
		buf[wr_len]='\0';
//		printf("test data is: %s\nwith length %d\n",buf,wr_len);
		ret = write(fd,buf,strtoul(argv[2], NULL, 0));
//		printf("write return %d\n",ret);
	}
	else
	{
		print_usage(argv[0]);
		return -1;
	}
	
	return 0;
}

