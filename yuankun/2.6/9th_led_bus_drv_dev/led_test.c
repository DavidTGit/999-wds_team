
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/* led_test on
 * led_test off
 */
int main(int argc, char **argv)
{
	int fd;
	int val = 1;

	if (argc < 3)
	{
		printf("Usage :\n");
		printf("%s led_num <on|off>\n", argv[0]);
		printf("led_num = 1,2,3 or 4\n");
		return 0;
	}
	
	fd = open("/dev/led", O_RDWR);
	if (fd < 0)
	{
		printf("can't open!\n");
	}
	
	if (strcmp(argv[2], "on") == 0)
	{
		val  = 1;
	}
	else
	{
		val = 0;
	}
	
	write(fd, &val, atoi(argv[1]));
	return 0;
}
