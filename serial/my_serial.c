#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "my_serial.h"

static int gsd=0;

int create_my_serial(const char* dev)
{
	int sd = open(dev,O_RDWR);
	if (sd <0) perror("open");
	gsd = sd;
	return gsd;
}

void close_my_serial()
{
	if (gsd>0) close(gsd);
}

int read_my_serial(char* buf,int len)
{
	int readlen = read(gsd,buf,len);
	if (readlen <0) perror("read");
	return readlen;
}

int poll_my_serial(int polltime)
{
	struct pollfd pollfds;
        pollfds.fd = gsd;
        pollfds.events = POLLIN;
	return poll(&pollfds,1,polltime);
}

int write_my_serial(char* buf,int len)
{
	int writelen=write(gsd,buf,len);
	if (writelen <0) perror("write");
	return writelen;
}


#if 0

int main()
{
	return 0;
}
#endif
