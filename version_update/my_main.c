#include "my_total_h.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef MAKE_FOR_CC
int main(int argc,char* argv[])
{
  if (argc != 4){
	printf("argc err\n");
	printf("board(smdkv210,...) initVer(1.0.00)  setType(Integer uboot:0,kernel:1,system:2)\n");
	return -1;
  }

  const char* board = argv[1];
  const char* initVer=argv[2];
  const char* setVerType = argv[3];
  SetVerType type;

  switch(atoi(setVerType)){
	case 0:type=U_BOOT;break;
	case 1:type=KERNEL;break;
	case 2:type=SYSTEM;break;
	default:printf("not support type\n");return -1;
  }

  char gitBranch[20];
  if (getCurGitBranch(gitBranch) == NULL){
	printf("getCurGitVer failed!\n");
	return -1;
  }

  char openVerName[50];
  getOpenVerName(type,openVerName);

  if (!is_need_update_ver(openVerName)){
	printf("don't need update\n");
	return 0;
  }

  update_txt_file_ver(board,type,gitBranch,initVer,openVerName);

  char lastestVer[20];
  if (getLastestVer(openVerName,lastestVer) == NULL){
	printf("getLastestVer failed!\n");
	return -1;
  }
  printf("lastestVer:%s\n",lastestVer);
  
  char kernelVer[30];
  switch(type)
  {
	case SYSTEM:
	case U_BOOT:strcpy(kernelVer,lastestVer);
	break;
	case KERNEL:sprintf(kernelVer,"%s %s",gitBranch,lastestVer);
	break;
  }

  if (setVersion(kernelVer,type)){
	printf("setVersion failed!\n");
  }

  return 0;
}
#endif

#ifdef MAKE_FOR_PYTHON
static PyMethodDef myModule_methods[] = {
	{"getOpenVerName", py_updateversion_getOpenVerName, METH_VARARGS},
	{NULL, NULL}
};

void initversionupdate()
{
	(void) Py_InitModule("versionupdate", myModule_methods);
}
#endif
