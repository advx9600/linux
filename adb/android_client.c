#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <poll.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


static int start_service(char* ip,int port);

int main(int argc,char* argv[])
{
/*
  if (argc != 3){
    printf("input connect ip,port\n");
    return -1;
  } */
  int times = 0;
  while (1){
    times++;
    start_service("114.215.252.143",5556);
    printf("restart service %d\n",times);
    sleep(8);
  }
  return 0;
}

static int create_local_sd()
{
// connect to 127.0.0.1:5555
// need to process  adb root exception
  struct sockaddr_in cliLocal;

  int sdLocal = socket(AF_INET, SOCK_STREAM, 0);
  if (sdLocal < 0){
    perror("socket local");
    return -1;
  }

  cliLocal.sin_family = AF_INET;
  cliLocal.sin_port = htons(5555);
  cliLocal.sin_addr.s_addr = INADDR_ANY;
  if (connect(sdLocal,(struct sockaddr*)&cliLocal,sizeof(cliLocal))){
    perror("connect local");
    return -1;
  }

  return sdLocal;
}

static int start_service(char* ip,int port)
{
  struct sockaddr_in cli,cliLocal;
  struct pollfd pfd[2];
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  char* conIp;
  int conPort;
  int len;
  char buf[4096];


  conIp = ip;
  conPort = port;

  if (sd < 0){
    perror("socket");
  }

  cli.sin_family = AF_INET;
  cli.sin_port = htons(conPort);
  cli.sin_addr.s_addr = inet_addr(conIp);

  if (connect(sd,(struct sockaddr*)&cli,sizeof(cli))){
    perror("connect");
    return -1;
  }

  
  int sdLocal = create_local_sd();

  while(1){
    pfd[0].fd = sd;
    pfd[0].events = POLLIN;

    pfd[1].fd = sdLocal;
    pfd[1].events = POLLIN;

    if (poll(pfd,2,3000) < 0){
      perror("poll");
      break;
    }
    if (pfd[0].revents & POLLIN){
      len = recv(pfd[0].fd,buf,sizeof(buf),0);
      if (len <= 0){
	printf("close sdlocal and sd\n");
	close(sdLocal);
	close(sd);
	return 0;
      }
      send(pfd[1].fd,buf,len,0);
    }
    if (pfd[1].revents & POLLIN){
      len = recv(pfd[1].fd,buf,sizeof(buf),0);
      if (len <= 0){
	printf("close sdlocal and sd 2\n");
	close(sdLocal);
	close(sd);
	return 0;
      }
      send(pfd[0].fd,buf,len,0);
    }
  }
  
  close(sdLocal);
  close(sd);
  return 0;
}
