#ifndef __PROCESS_PARRENT__
#define __PROCESS_PARRENT__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <sys/wait.h>
#include <math.h>

#define max_devices 10
#define max_data_len 20

typedef struct spidev{
char * name;
int data[max_data_len];
}SPI_DEVICE;


#endif
