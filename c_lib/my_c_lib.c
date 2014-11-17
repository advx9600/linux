#include "my_c_lib.h"

static char* ver_add_2(char* ver,int pos,int val)
{
  int i,j=0;

  for (i=strlen(ver)-1,j=0;i>-1;i--)
  {
        if (ver[i] >= '0' && ver[i] <= '9'){
          j++;
        }
        if (j-1 == pos){
          break;
        }
  }

  if (i<0) return NULL;

  int isOver= ((ver[i]-'0')&0xff)+val>9?1:0;
  ver[i] = isOver?ver[i]+val-10:ver[i]+val;

  if (isOver) ver_add_2(ver,pos+1,1);
  return ver;
}

char* ver_add(char* outVer,int num)
{
  int i,x;
  i=0;
  while(1)
  {
        x = num%10;
        num = num/10;
        if (x == 0 && num== 0) break;
        ver_add_2(outVer,i,x);
        i++;
  }
  return outVer;
}

char* getCurGitBranch(char* ver)
{
  //char* result = "";
  int isFind;
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

#if 0
int main()
{
  char buf[100];
  printf("%s\n",getCurGitBranch(buf));
  return 0;
}
#endif
