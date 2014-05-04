#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void replaceFirst(char *str1,const char *str2,const char *str3)  
{  
    char str4[strlen(str1)+1];  
    char *p;  
    strcpy(str4,str1);  
    if((p=strstr(str1,str2))!=NULL)/*p指向str2在str1中第一次出现的位置*/  
    {  
        while(str1!=p&&str1!=NULL)/*将str1指针移动到p的位置*/  
        {  
            str1++;  
        }  
        str1[0]='\0';/*将str1指针指向的值变成/0,以此来截断str1,舍弃str2及以后的内容，只保留str2以前的内容*/  
        strcat(str1,str3);/*在str1后拼接上str3,组成新str1*/  
        strcat(str1,strstr(str4,str2)+strlen(str2));/*strstr(str4,str2)是指向str2及以后的内容(包括str2),strstr(str4,str2)+strlen(str2)就是将指针向前移动strlen(str2)位，跳过str2*/  
    }  
}  
/*将str1出现的所有的str2都替换为str3*/  
static void replace(char *str1,const char *str2,const char *str3)  
{  
    while(strstr(str1,str2)!=NULL)  
    {  
        replaceFirst(str1,str2,str3);  
    }  
}  

static char* toUpperCase(const char* str,char* upperStr)
{
	int i;
	for (i=0;i<strlen(str);i++) upperStr[i]=toupper(str[i]);
	return upperStr;
}

int main(int argc,char* argv[])
{
	if (argc !=  3){
		printf("argc err!\n");
		printf("filePath className\n");
		return -1;
	}
	const char* filePath=argv[1];
	const char* className=argv[2];
	char cppName[1024];
	char incName[1024];
	
	strcpy(cppName,filePath);
	if (cppName[strlen(cppName)-1]!='/')
		strcat(cppName,"/");
	strcat(cppName,className);
	strcpy(incName,cppName);
	
	strcat(cppName,".cpp");
	strcat(incName,".h");
	
	if (fopen(cppName,"rb") || fopen(incName,"rb")){
		printf("file already exist:\
			path:%s,class:%s\n",filePath,className);
		return -1;
	}
	
	FILE* fdCpp = fopen(cppName,"wb");
	FILE* fdInc = fopen(incName,"wb");
	FILE* fdIncSample = fopen("HelloWorldScene.h","rb");
	FILE* fdCppSample = fopen("HelloWorldScene.cpp","rb");
	char readBuf[1024*2];
	if (fdIncSample == NULL || fdCppSample == NULL){
		printf("sample file not exist!\n");
		return -1;
	}
	
	while( !feof(fdIncSample) ){
	   char upperName[100];
	   if (fgets(readBuf,sizeof(readBuf),fdIncSample) == NULL) continue;
	   replace(readBuf,"HELLOWORLD_SCENE",toUpperCase(className,upperName));
	   replace(readBuf,"HelloWorld",className);
	   fwrite(readBuf,1,strlen(readBuf),fdInc);
	}


	while( !feof(fdCppSample) ){
	   if (fgets(readBuf,sizeof(readBuf),fdCppSample) == NULL) continue;
	   replace(readBuf,"HelloWorldScene",className);
	   replace(readBuf,"HelloWorld",className);
	   fwrite(readBuf,1,strlen(readBuf),fdCpp);
	}

	fclose(fdCpp);
	fclose(fdInc);
	fclose(fdIncSample);
	
	return 0;
}
