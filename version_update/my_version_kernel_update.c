#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_total_h.h"

char* getLastestVer(const char* inFile,char* ver)
{
  FILE* fd =fopen(inFile,"r");
  if (fd == NULL){
        perror("fopen");
        return NULL;
  }

  int i,j;
  char buf[1024];
  int isFound=0;
  while( !feof(fd) )
  {
	char* pAt;
        if (fgets(buf,sizeof(buf),fd) == NULL) continue;
	if (!(pAt=strstr(buf,"latestVer"))) continue;
	isFound =1;
	int start=0;
	for (i=0,start=0;i<strlen(pAt);i++)
	{
	  if (!start && pAt[i] >='0' && pAt[i] <= '9'){
		start = i;
	  }
	  if (start){
		if (pAt[i] == ' ' || pAt[i] == ',' || pAt[i] == '\n'){
		  pAt[i]='\0';
		  break;
		}
	  }
	}
	strcpy(ver,pAt+start);
	break;
  }

  fclose(fd);

  if (isFound) return ver;
  return NULL;
}

char* getCurGitBranch(char* ver)
{
  char* result = "";
  int i,isFind;
  FILE *fpRead;
  const char* command="git branch";

  fpRead = popen(command, "r");
  char buf[2048];
  memset(buf,'\0',sizeof(buf));

  isFind = 0;
  while(fgets(buf,sizeof(buf)-1,fpRead)!=NULL)
  {
	if (buf[0] == '*'){
	  strcpy(ver,buf+2);
	  ver[strlen(ver)-1]='\0';
	  isFind = 1;
	  break;
	}
  }

  if(fpRead!=NULL) pclose(fpRead);

  if (isFind) return ver;

  return NULL;
}

static char* kernelModifyLine(char* buf,const char* ver)
{
  int i=0;
  for (i=0;i<strlen(buf);i++)
  {
	if (!strncmp(buf+i,"kernel:?",8)){
	  char temp[1024];
	  strcpy(temp,buf+i+8);
	  buf[i+0]='\0';
	  strcat(buf,ver);
	  strcat(buf,temp);
	  return buf;
	}
  }

  return NULL;
}

int setVersion(const char* ver,SetVerType type)
{
  FILE* fd; 
  FILE* fdOut;

  switch(type){
	case KERNEL:
	fd = fopen("fs/proc/version.sample.c","rb");
	fdOut = fopen("fs/proc/version.c","wb");
	break;

	case U_BOOT:
	fd = fopen("include/configs/smdkv210single.sample.h","rb");
	fdOut = fopen("include/configs/smdkv210single.h","wb");
	break;
	case SYSTEM:
	fd = fopen("build/tools/buildinfo.sample.sh","r");
	fdOut = fopen("build/tools/buildinfo.sh","w");
  }

  if (fd == NULL || fdOut == NULL){
	perror("fopen");
	return -1;
  }


  char buf[1024];
  int i,isFound=0;
  while( !feof(fd) )
  {
	if (fgets(buf,sizeof(buf),fd) == NULL) continue;

	if (!isFound)
		isFound=kernelModifyLine(buf,ver)?1:0;

	fwrite(buf,1,strlen(buf),fdOut);
  }

  fclose(fd);
  fclose(fdOut);
  return 0;
}
