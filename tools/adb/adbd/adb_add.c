#include "adb.h"

int property_get(const char *key, char *value, const char *default_value)
{
    //printf("get name:%s\n",key);
    if (!strcmp(key,"persist.adb.trace_mask")){
	strcpy(value,"1");
    }else if (!strcmp(key,"service.adb.tcp.port")){
	strcpy(value,"0");
    }else if (!strcmp(key,"ro.secure")){
	strcpy(value,"0");
    }else if (!strcmp(key,"ro.adb.secure")){
	strcpy(value,"0");
    }else if (!strcmp(key,"ro.kernel.qemu")){
	strcpy(value,"0");
    }else if (!strcmp(key,"ro.product.name")){
	strcpy(value,"smdk4x12");
    }else if (!strcmp(key,"ro.product.model")){
	strcpy(value,"smdk4x12-rp4412");
    }else if (!strcmp(key,"ro.product.device")){
	strcpy(value,"smdk4x12");
    }else{
	printf("not support property:%s\n",key);
    }
    return 0;
}
int property_set(const char *key, const char *value)
{
    printf("set name:%s\n",key);
    return 0;
}
int backup_service(BackupOperation operation, char* args){return 0;}
int qemu_pipe_open(void* con_name) { return 0; }
