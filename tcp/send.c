#include<netinet/in.h>  // sockaddr_in 
#include<sys/types.h>  // socket 
#include<sys/socket.h>  // socket 
#include<stdio.h>    // printf 
#include<stdlib.h>    // exit 
#include<string.h>    // bzero 
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
  
  
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}  

int main(int argc,char* argv[])
{ 
  if (argc != 2){
    printf("inputFileName\n");
    return -1;
  }
  char* file_name = argv[1];
  // 声明并初始化一个客户端的socket地址结构 
  struct sockaddr_in client_addr; 
  bzero(&client_addr, sizeof(client_addr)); 
  client_addr.sin_family = AF_INET; 
  client_addr.sin_addr.s_addr = htons(INADDR_ANY); 
  client_addr.sin_port = htons(0); 
  
  // 创建socket，若成功，返回socket描述符 
  int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
  if(client_socket_fd < 0) 
  { 
    perror("Create Socket Failed:"); 
    exit(1); 
  } 

// 声明一个服务器端的socket地址结构，并用服务器那边的IP地址及端口对其进行初始化，用于后面的连接 
  struct sockaddr_in server_addr; 
  bzero(&server_addr, sizeof(server_addr)); 
  server_addr.sin_family = AF_INET; 
  server_addr.sin_port = htons(5560); 
  server_addr.sin_addr.s_addr=inet_addr("115.28.179.54");
  socklen_t server_addr_length = sizeof(server_addr); 
  
  // 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接 
  if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0) 
  { 
    perror("Can Not Connect To Server IP:"); 
    exit(0); 
  } 
  
  FILE *fp = fopen(file_name, "r"); 
  if(NULL == fp) 
  { 
    printf("File:\t%s Can Not Open To Write\n", file_name); 
    exit(1); 
  } 

  int num =0;
  char buf[1024];
  int filesize =get_file_size(file_name);
  int down_num = 0;
  while ( (num = fread(buf,1,1024,fp)) > 0){
    if (send(client_socket_fd,buf,num,0)< 1) 
    {
      perror("send");
      break;
    }
    down_num += num;
    printf("percent %d\n",down_num*100/filesize);
  }

  fclose(fp);
  close(client_socket_fd);
  
  return 0;
}
