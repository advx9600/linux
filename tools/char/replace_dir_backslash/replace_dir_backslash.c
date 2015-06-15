#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 replace back slash
*/
int main(int argc,char* argv[])
{
    if (argc != 2){
	printf("dir:must add \"\" \n");
	return -1;
    }

    int i;
    char* dir = argv[1];

    for (i = 0;i < strlen(dir);i++)
    {
	if (dir[i] == '\\') dir[i]='/';
    }

    printf("%s\n",dir);

    return 0;
}
