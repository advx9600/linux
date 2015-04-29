#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termio.h>
#include <errno.h>
#include <pthread.h>
#include <poll.h>
#include <signal.h>


#define LOGE printf
#define LOGI printf
#define LOGW printf

static int getBaudRate(int baudRate) {
	switch (baudRate) {
	case 0:
		return B0;
	case 50:
		return B50;
	case 110:
		return B110;
	case 134:
		return B134;
	case 150:
		return B150;
	case 200:
		return B200;
	case 300:
		return B300;
	case 600:
		return B600;
	case 1200:
		return B1200;
	case 1800:
		return B1800;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	}
}

 int pollRead(int sd, int msec) { // timeout or err return 0 ,have data return non-zero
	int ret = 0;
	struct pollfd fds;
	fds.fd = sd;
	fds.events = POLLIN;
	ret = poll(&fds, 1, msec);
	if (ret == -1) {
		perror("poll");
		LOGE("poll read failed!");
		ret = 0;
	}
	return ret;
}

 int openSerial(const char* dev, int baudRate) {
	struct termios option;

	int fd = open(dev, O_RDWR);
	if (fd < 0) {
		perror("open device");
		LOGE("open %s failed!", dev);
		return -1;
	}

	bzero(&option, sizeof(option));
	cfmakeraw(&option);

	option.c_cflag |= CLOCAL | CREAD;
	option.c_cflag |= CS8;
	option.c_cflag &= ~PARENB; // no parity check
	option.c_cflag &= ~CSTOPB; // one stop bit
	option.c_oflag = 0;
	option.c_lflag |= 0;
	option.c_oflag &= ~OPOST;
	option.c_cc[VTIME] = 1; /* unit: 1/10 second. */
	option.c_cc[VMIN] = 1; /* minimal characters for reading */
	tcflush(fd, TCIFLUSH);
	cfsetispeed(&option, getBaudRate(baudRate));
	cfsetospeed(&option, getBaudRate(baudRate));
	if (tcsetattr(fd, TCSANOW, &option)) {
		LOGE("tcsetattr failed!");
	}
	LOGI("open fd:%d", fd);
	return fd;
}

 void closeSerial(int fd) {
	LOGI("close fd:%d", fd);
	if (fd > 0) {
		close(fd);
	}
}

 int writeData(int fd_write, const char* bts,
		int leng) {
//	LOGI("start writeData");
	if (bts == NULL) {
		LOGE("write bts is NULL!");
		return 0;
	}
	if (write(fd_write, bts, leng) != leng) {
		LOGE("write data failed!");
		return 0;
	}
	return leng;
}

 int readData(int fd_read,char*bts,
		int leng, int timeOut) {
	int i;
	int ret = 0;

	if (!pollRead(fd_read, timeOut)) {
		return 0;
	}
	ret = read(fd_read, bts,leng);
	if (ret < 0) {
		LOGE("read failed!");
	}
	return ret;
}

#if 0
int main(int argc,char* argv[])
{
	if (argc != 2){
		printf("argc err\n");
		printf("deviceName\n");
		return -1;
	}

	const char* dev= argv[1];
	int fd =  openSerial(dev,115200);
	
	int count=0;
	while(1)
	{
		sleep(1);
		count++;
		char buf[10];
		buf[0]='a';
		buf[1]='\n';
		writeData(fd,buf,2);
		printf("%d serial write\n",count);
	}

	closeSerial(fd);
	return 0;
}
#endif
