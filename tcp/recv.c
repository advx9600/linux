#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
  if (argc != 2){
    printf("outputfile \n");
    return -1;
  }
  const  char* file_name=argv[1];

  struct  sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));  
  server_addr.sin_family = AF_INET;  
  server_addr.sin_addr.s_addr  =INADDR_ANY;
  server_addr.sin_port = htons(5560);


  int server_socket = socket(AF_INET, SOCK_STREAM, 0);  
  if (server_socket < 0)  
  {  
    perror("Create Socket Failed!\n");  
    exit(1);  
  }  
  
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))  
  {  
    perror("Server Bind Port:");  
    exit(1);  
  }
    // server_socket用于监听   

    if (listen(server_socket,1)) 
    {  
        perror("Server Listen Failed!\n");  
        exit(1);  
    }  


  struct sockaddr_in client_addr;  
  socklen_t          length = sizeof(client_addr);  
  int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
  if (new_server_socket < 0)  
  {  
    perror("Server Accept Failed!\n");  
    exit(1);
  }

  char buf[1024];
  int num;
  FILE* fd = fopen(file_name,"wb");
  if (fd == NULL){
    perror("fopen");
    return -1;
  }
  while ((num=recv(new_server_socket,buf,1024,0)) > 0){
    if (fwrite(buf,1,num,fd) != num){
      perror("fwrite");
      exit(-1);
    }
  }

  fclose(fd);
  close(new_server_socket);
  close(server_socket);
  return 0;
}
