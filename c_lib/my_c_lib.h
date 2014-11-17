#ifndef MY_C_LIB__H
#define MY_C_LIB__H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
  ver_add("1.3.1",1) == 1.3.2
*/
char* ver_add(char* outVer,int num);
/*
得到当前Git版本的名称(master...)
*/

char* getCurGitBranch(char* ver);
#endif
