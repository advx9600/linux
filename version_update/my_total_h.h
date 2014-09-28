#include <Python.h>
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
int update_txt_file_ver(const char* board,SetVerType type, \
	const char* gitBranch, \
        const char* baseVer, const char* writeFile);

/* my_version_kernel_update.c */
#define MY_DIR "my_dir"
char* getLastestVer(const char* inFile,char* ver);
char* getCurGitBranch(char* ver);
int setVersion(const char* ver,SetVerType type);
char* getOpenVerName(SetVerType type,char* openVerName);

PyObject* py_updateversion_getOpenVerName(PyObject* self, PyObject* args);
#endif
