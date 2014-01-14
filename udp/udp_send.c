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

#include "udp_send.h"

int gsd=0;

int create_udp_socket(int port)
{
        int sd=socket(AF_INET,SOCK_DGRAM,0);
        if (sd <0) {
        perror("socket");
        return -1;
        }
	gsd=sd;


	if (port < 1 || port >65536)	return -1;

        struct sockaddr_in addr;

        addr.sin_family =AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr =INADDR_ANY;

        if (bind(sd,(struct sockaddr*)&addr,sizeof(addr))){
        perror("bind");
        return -1;
        }

        int re_use =1;
        if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&re_use,sizeof(re_use))){
        perror("setsockopt");
        return -1;
        }

	return 0;
}

int read_udp_data(char* data,int len)
{
	int readlen=read(gsd,data,len);
	if (readlen <0) perror("read");
	return readlen;
}

int send_udp_data(const char* data ,int len,const char* ip,int port)
{
	struct sockaddr_in addr;
	int sendlen;
	addr.sin_family=AF_INET;
	addr.sin_port =htons(port);
	addr.sin_addr.s_addr=inet_addr(ip);
	sendlen=sendto(gsd,data,len,0,(struct sockaddr*)&addr,sizeof(addr));
	if (sendlen !=len ) perror("sendto");
	return sendlen;
}

void close_udp_socket()
{
	close(gsd);
}

#if 1

void read_loop(int port)
{
	char data[1024*1024];
	create_udp_socket(port);
	while(1)
	{
		int len=read_udp_data(data,sizeof(data));
		if (len==1 && data[0]=='q') break;
		data[len]='\0';
		printf("read len:%d,data:%s\n",len,data);
	}
	close_udp_socket();
}
void send_loop(const char*ip,int port)
{
	char data[1024*1024];
	int i,j;
	create_udp_socket(0);

	for (i=1;i<10;i++)
	{
		for (j=0;j<i;j++) data[j]='0'+j;
		send_udp_data(data,i,ip,port);
		sleep(1);
	}
	data[0]='q';
	send_udp_data(data,1,ip,port);
	close_udp_socket();
}
int main(int argc,char* argv[])
{
	if (argc <2 || (argv[1][0]!='s' && argv[1][0]!='r')){
		printf("s ip port send, r port recv\n");
		return -1;
	}
	switch(argv[1][0])
	{
	case 's':send_loop(argv[2],atoi(argv[3]));break;
	case 'r':read_loop(atoi(argv[2]));break;
	}
	return 0;
}
#endif
