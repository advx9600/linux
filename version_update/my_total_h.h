#include <stdio.h>
#ifndef MY_TOTAL_H_FILE
#define MY_TOTAL_H_FILE

typedef enum{
U_BOOT=0,
KERNEL,
SYSTEM,
} SetVerType;

/* my_version_check.c */
int is_need_update_ver(const char* verFile);
int update_txt_file_ver( const char* gitBranch, \
        const char* baseVer, const char* writeFile);

/* my_version_kernel_update.c */
char* getLastestVer(const char* inFile,char* ver);
char* getCurGitBranch(char* ver);
int setVersion(const char* ver,SetVerType type);
#endif
