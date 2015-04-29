#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{

    const char* file = "/home/abc/Public/test/save.txt";
    char buf[1024];
    int len;

    FILE* fd = freopen(file,"w",stderr);
    if (fd == NULL){perror("freopen");return -1;}

    system("make");

    fclose(fd);

    fd = fopen(file,"r");
    if (fd == NULL) {perror("fopen");return -1;}

    while( !feof(fd) ){
	if (fgets(buf,sizeof(buf),fd)){
	    buf[sizeof(buf)-2]=0;
	    if (strstr(buf,"error") || strstr(buf,"undefined"))
		printf("\033[31m""%s""\033[m", buf);
	}
    }

    fclose(fd);

    return 0;
}
