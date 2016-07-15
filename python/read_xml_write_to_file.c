#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int findStr(const char* file,char* tag,char* val);
int writeToFile(char* tag,char* eng,char* zh,char* tw);

int main(int argc,char* argv[])
{

  if (argc != 2){
	printf("input tag\n");
	return -1;
  }

  char* tag = argv[1];

  const char file[] = "/mnt/hgfs/Desktop/temp/BoYuanSetting/res/values/strings.xml";
  const char file2[] = "/mnt/hgfs/Desktop/temp/BoYuanSetting/res/values-zh-rCN/strings.xml";
  const char file3[] = "/mnt/hgfs/Desktop/temp/BoYuanSetting/res/values-zh-rTW/strings.xml";

  char engVal[100];
  char zhVal[100];
  char twVal[100];
  
  findStr(file,tag,engVal);
  findStr(file2,tag,zhVal);
  findStr(file3,tag,twVal);

  printf("enVal:%s\nzhVal:%s\ntwVal:%s\n\n",engVal,zhVal,twVal);


  printf("is wirte to file Y/N\n");
  char c=getchar();
  if (c == 'y' || c == 'Y'){
    writeToFile(tag,engVal,zhVal,twVal);
  }

  return 0;
}


static int writeToFileB(const char* file,char* tag,char* val)
{
  FILE* fd = fopen(file,"r+");
  if (fd == NULL){ perror("fopen");return -1;}


  if (fseek(fd,-100,SEEK_END)){
    perror("fseek ");
  }

  char buf[200];
  char* resend= "</resources>";

  while( !feof(fd) ){
   if (fgets(buf,sizeof(buf)-1,fd) ){
	if (strstr(buf,resend)){

	  if (fseek(fd,-strlen(buf),SEEK_CUR)){
		perror("fseek");
		break;
	  }

	  sprintf(buf,"	<string name=\"%s\">%s</string>\n",tag,val);

	  if (fwrite(buf,1,strlen(buf),fd) != strlen(buf)){
		perror("fwrite");
	  }

	  if (fwrite(resend,1,strlen(resend),fd) != strlen(resend)){
		perror("fwrite");
	  }

	  break;
	}
    }
  }

  fclose(fd);

  return 0;
}

int writeToFile(char* tag,char* eng,char* zh,char* tw)
{
  const char file[] = "/mnt/hgfs/Desktop/temp/TvdSettings/res/values/strings.xml";
  const char file2[] = "/mnt/hgfs/Desktop/temp/TvdSettings/res/values-zh-rCN/strings.xml";
  const char file3[] = "/mnt/hgfs/Desktop/temp/TvdSettings/res/values-zh-rTW/strings.xml";

  char val[50]="";

  findStr(file,tag,val);
  if (strlen(val) != 0){
	printf("%s already exist\n",tag);
	return -1;
  } 

  writeToFileB(file,tag,eng);
  writeToFileB(file2,tag,zh);
  writeToFileB(file3,tag,tw);
  return 0;
}

int findStr(const char* file,char* oldtag,char* val)
{
 

  int i,j;
  char tag[100];

  char buf[1024];

  sprintf(tag,"\"%s\"",oldtag);
  
  FILE* fd = fopen(file,"r");

  if (fd == NULL) {perror("fopen");return -1;}
 
  while( fgets(buf,sizeof(buf)-1,fd) )
  {
    if (strstr(buf,tag)){
	int start = 0;
	j=0;
	for (i=0;i<strlen(buf);i++)
	{

	  if (start) {
	    if (buf[i] == '<'){
		val[j]=0;
		break;
	    }

	    val[j++]=buf[i];
	  }

	  if (buf[i] == '>'){
	    start = 1;
	  }

	}
	break;
    }
  }

  fclose(fd);

  return 0;
}
