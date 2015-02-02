#include <stdio.h>
#include <fcntl.h>
#include <string.h>

typedef struct write_a{
int ops;
char name[20];
int addr;
int val;
}Write_a;

int main()
{
    int fd = open("/dev/pin-ssd2828",O_RDWR);
    if (fd < 0){
	perror("open");
	return -1;
    }

    Write_a data;

    data.ops=0;
    data.val = 1;
    while(1)
    {
	
	data.ops=0;
	data.val = !data.val;
	write(fd,&data,sizeof(data));
	sleep(3);
    }

    close(fd);

    return 0;
}
