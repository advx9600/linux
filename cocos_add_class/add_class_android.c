#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc,char* argv[])
{
	if (argc !=  3){
		printf("argc err!\n");
		printf("Android.mk className\n");
		return -1;
	}

	const char* mkPath=argv[1];
	const char* className=argv[2];
	char cppName[1024];
	char addBuf[1024*2];
	char fileBuf[1024*1024];
	char fileBufAppend[1024*1024];
	int fileLen=0;
	
	strcpy(cppName,"../../Classes/");
	strcat(cppName,className);
	strcat(cppName,".cpp \\\n");
	
	FILE* fd= fopen(mkPath,"r+");
	if (fd == NULL){
		perror("fopen");
		return -1;
	}

	fileLen = fread(fileBuf,1,sizeof(fileBuf),fd);
	char* appendChar = strstr(fileBuf,"LOCAL_SRC_FILES");
	char tempChar;
	while( *(appendChar++) !='\n');
	tempChar = *appendChar;
	*appendChar = '\0';

	rewind(fd);
	fwrite(fileBuf,1,strlen(fileBuf),fd);
	fwrite(cppName,1,strlen(cppName),fd);
	*appendChar = tempChar;
	fwrite(appendChar,1,strlen(appendChar),fd);

	fclose(fd);
	return 0;
}
