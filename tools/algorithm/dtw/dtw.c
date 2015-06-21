#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
http://www.bubuko.com/infodetail-465930.html
*/

#define INT_MAX ((int)(~0U>>1))

#ifndef print
#define print printf
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#endif

int dwt_algorithm(const int *p,const int pN,const int* q,const int qN);


static void print_array(int* val,int row,int col)
{
    int i,j;
    const char* format = "%04d  ";
    printf("      ");
    for (i = 0; i< row ;i++) printf(format,i); printf("\n\n");

    for (i = 0; i< col ;i++)
    {
	for (j = 0; j< row; j++)
	{
	    if (j == 0 ) printf(format,i);
	    printf(format,val[i*row+j]);
	}
	printf("\n");
    }
	
}
int dwt_algorithm(const int *p,const int pN,const int* q,const int qN)
{
    int i,j,k;
    unsigned int sum[2];

    for (i = 0,j = 0,k = 0,sum[0] = (p[j]-q[i])*(p[j]-q[i]),sum[1] = 0;\
	 i != qN -1 || j != pN -1;k++)
    {
	int a = i+1>qN-1?-1:abs(p[j]-q[i+1]); // right
	int b = j+1>pN-1?-1:abs(p[j+1]-q[i]); // down
	int c = i+1>qN-1?-1:j+1>pN-1?-1:abs(p[j+1]-q[i+1]);

	//printf("a:%d,b:%d,c:%d\n",a,b,c);
	if (c > -1 && (a < 0 || c <=a ) && (b < 0 || c <= b)) {i++;j++;}
	else
	if (a > -1 && (b < 0 || a <=b ) && (c < 0 || a <= c)) {i++;}
	else 
	if (b > -1 && (a < 0 || b <= a) && (c < 0 || b <= c)) {j++;}
	int w= (p[j]-q[i])*(p[j]-q[i]);
	if (sum[0] > sum[0] +w ){
	    sum[1] += 1;
	}
	sum[0] += w;
    }

    if (sum[0] != 0 || sum[1] != 0 && k != 0){
	return ((0xfffffff)/k)*sum[1]+sum[0]/k;
    }

    return 0;
}

static int get_sample_fd(FILE* fd , int* p ,int* pN)
{
    int j,k;
    short readVal[32];
    const int limit = 300;
    int start=0;

    k = 0;
    int overTimes = 0;
    while( !feof(fd) )
    {
	const int len =fread(readVal,2,ARRAY_SIZE(readVal),fd);
	int max = 0;
	for (j = 0 ; j < len; j++)
	{
	    int val = abs(readVal[j]);
	    if (max < val ) max = val;
	}

//       printf("max:%d\n",max);
	if (start && max < limit){
	    overTimes ++;
	    if (overTimes == 3){
		break;
	    }
	}
 
	if ( max > limit ){
	    start = 1;
	    overTimes =0;
	}
	
	if (start){
	    for (j = 0 ; j < len; j++)
	    {
		p[k++] = readVal[j];
	    }
	}
    }

    *pN = k;

    return 0;
}

static int get_sample_file(const char* file,int* p, int* pN)
{
    int i,j,k;

    FILE* fd = fopen(file,"r");   
    if (fd == NULL) { perror("fopen");return -1;}
    
    get_sample_fd(fd,p,pN);
    
    fclose(fd);

    return 0;
}

static int save_buf_file(const char* file,const int*p ,const int pN)
{
    FILE* fd = fopen(file,"w");
    if (fd == NULL) { perror("fopen"); return -1;}

    short buf[1024];
    int i,j;
    const int shortNum = ARRAY_SIZE(buf);

    for (i = 0,j = 0; i < pN ; i++)
    {
        buf[ i %shortNum ] = p[i];
	if ( i % shortNum == shortNum -1){
	    fwrite(buf,2,shortNum,fd);
	}else if ( i ==  pN-1 ){
	    fwrite(buf,2,i%shortNum + 1,fd);
	}
    }

    fclose(fd);
    return 0;
}

static int compare_file(const char* openFile,int* p, int pN)
{
    FILE* fd = fopen(openFile,"r");
    if (fd == NULL) { perror("fopen"); return -1; }

    int i,qN;
    int* q = (int*) malloc(1024*1024);
    if (q == NULL) { perror("malloc"); return -1; }


    i = 0;

    int min = 0xFFFFFF;
    int fPos = 0;
    while ( !feof(fd) )
    {
	get_sample_fd(fd,q,&qN);

	if (qN > pN -pN/10 && qN < pN + pN/10){
	    i++;

    	    char file[1024];
	    const char* format="/mnt/hgfs/Desktop/temp/autio_process/three/%03d.pcm";
	    sprintf(file,format,i);

	    int w = dwt_algorithm(p,pN,q,qN);
	    if ( min > w ) { 
		min = w; 
		fPos = ftell(fd) ;

	    save_buf_file(file,q,qN);

		printf("i:%2d,w:%8d,ftell:%8d\n",i,w,fPos);
	    }
	}
    }

    fclose(fd);
    return 0;
}

int main()
{
    int* p = (int*) malloc(1024*1024);
    int pN;
    get_sample_file("/mnt/hgfs/Desktop/temp/autio_process/three/00_sample_2.pcm",p,&pN);
//    printf("pN:%d\n",pN);
    save_buf_file("/mnt/hgfs/Desktop/temp/autio_process/three/00_sample_output.pcm",p,pN);
 //   compare_file("/mnt/hgfs/Desktop/temp/autio_process/three/Lesson2.pcm",p,pN);
    compare_file("/mnt/hgfs/Desktop/temp/autio_process/three/Lesson3.pcm",p,pN);

    free(p);
    return 0;
}
