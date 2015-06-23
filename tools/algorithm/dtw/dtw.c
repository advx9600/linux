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


static void print_array(const int* p,int pN,const int* q,int qN)
{
    int i,j;
    const char* format = "%8d  ";
    printf("          ");
    for (i = 0; i< qN ;i++) printf(format,i); printf("\n\n");

    for (i = 0; i< pN ;i++)
    {
        for (j = 0; j< qN; j++)
        {
            if (j == 0 ) printf(format,i);
            printf(format, abs(p[i]-q[j]) );
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

static int gPosBefore;

static int get_sample_fd(FILE* fd , int* p ,int* pN)
{
    int j,k;
    short readVal[32];
    const int limit = 300;
    int start=0;

    k = 0;
    int overTimes = 0;
    int isIgnore = 0;
    gPosBefore = -1;
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
	    if (gPosBefore < 1 ) gPosBefore = ftell(fd);
	}
	
	if (start){
	    if ( k + len > 1024*1024/4 -1 ){ isIgnore =1; k = 0;}
	    if ( !isIgnore )
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

static int compare_file(const char* openFile,int* p, int pN,int* fPos)
{
    FILE* fd = fopen(openFile,"r");
    if (fd == NULL) { perror("fopen"); return -1; }

    int i,qN;
    qN = 1024*1024;
    int* q = (int*) malloc(qN);
    if (q == NULL) { perror("malloc"); return -1; }


    i = 0;

    int min = 0xFFFFFF;

    while ( !feof(fd) )
    {
	get_sample_fd(fd,q,&qN);

	if (qN > pN -pN/10 && qN < pN + pN/10){
	    i++;

    	    char file[1024];
	    const char* format="/mnt/hgfs/Desktop/temp/autio_process/three/%03d.pcm";
	    sprintf(file,format,i);

	    //int w = compare_squares(p,pN,q,qN);
	    int w = dwt_algorithm(p,pN,q,qN);
	    if ( min > w ) { 
		min = w; 
		*fPos = gPosBefore;

//	    save_buf_file(file,q,qN);

		printf("i:%2d,w:%8d,ftell:%8d\n",i,w,*fPos);
	    }
	}
    }

    fclose(fd);
    return 0;
}

int main()
{
    int i;
    const char* dir = "/mnt/hgfs/Desktop/temp/autio_process/japan_advance_level/";
    const char* dirOut = "/mnt/hgfs/Desktop/temp/autio_process/three/";

    int* p = (int*) malloc(1024*1024);
    int pN;
    get_sample_file("/mnt/hgfs/Desktop/temp/autio_process/three/00_sample_2.pcm",p,&pN);
//    printf("pN:%d\n",pN);
//    save_buf_file("/mnt/hgfs/Desktop/temp/autio_process/three/00_sample_output.pcm",p,pN);
    #if 1

    for ( i = 0 ; i < 24; i++)
    {
	char cmd[1024];
 	char mp3File[512];
	char pcmFile[512];
	char splitWav1[512];
	char splitWav2[512];
	char splitMp31[512];
	char splitMp32[512];
	int fPos=-1;

	sprintf(mp3File,"/mnt/hgfs/Desktop/temp/autio_process/japan_advance_level/Lesson%d.mp3",i+1);
        sprintf(pcmFile,"/mnt/hgfs/Desktop/temp/autio_process/three/Lesson%d.pcm",i+1);
	sprintf(cmd,"./decode_pcm %s %s",mp3File,pcmFile);

	printf("%s\n",cmd);
	system(cmd);

	compare_file(pcmFile,p,pN,&fPos);

	if (fPos < 0 ){ printf("search failed!\n"); continue; }


	sprintf(splitWav1,"/mnt/hgfs/Desktop/temp/autio_process/three/Lesson%d_1.wav",i+1);
	sprintf(splitWav2,"/mnt/hgfs/Desktop/temp/autio_process/three/Lesson%d_2.wav",i+1);
	sprintf(cmd,"./pcm_to_wav %s %s %s %d",mp3File,splitWav1,splitWav2,fPos);
	printf("%s\n",cmd);
	system(cmd);

	
	sprintf(splitMp31,"/mnt/hgfs/Desktop/temp/autio_process/four/%02d.mp3",i*2+1);
	sprintf(splitMp32,"/mnt/hgfs/Desktop/temp/autio_process/four/%02d.mp3",i*2+2);
	sprintf(cmd,"lame %s %s",splitWav1,splitMp31);
	system(cmd);
	sprintf(cmd,"lame %s %s",splitWav2,splitMp32);
	system(cmd);
    }

    #endif

    free(p);
    return 0;
}
