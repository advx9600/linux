#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int set_color(int width,int height,int r,int g,int b)
{
    int i;
    int size=width*height*sizeof(int);

    int fd = open("/dev/fb0",O_RDWR);
    if (fd <0){ perror("open");return -1;}

    void* ptr = mmap(0, size,
                        PROT_WRITE | PROT_READ,
                        MAP_SHARED, fd, 0);
    if (ptr== MAP_FAILED){ perror("mmap");return -1; }


    for (i=0;i<size;i=i+4)
    {
	*(char*)(ptr+i+0)=r; // blue
	*(char*)(ptr+i+1)=g; // green
	*(char*)(ptr+i+2)=b; // red
	*(char*)(ptr+i+3)=255; // may be transparent
    }

    munmap(ptr,size);
    close(fd);
    return 0;
}

#if 0
int main(int argc,char* argv[])
{
    if (argc != 4){
	printf("argc err!\n");
	printf("r g b\n");
    }
    int r=atoi(argv[1]);
    int g=atoi(argv[2]);
    int b=atoi(argv[3]);
    set_color(1024,600,r,g,b);
    return 0;
}
#endif
