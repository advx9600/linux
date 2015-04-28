#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>

#define MAX_SIZE 4096

#define KEY_NUM 0x1236

typedef struct shared_use_st
{
     int size;
     char text[MAX_SIZE];
}Shared_use_st;


static int pollRead(int sd, int msec) { // timeout or err return 0 ,have data return non-zero
	int ret = 0;
	struct pollfd fds;
	fds.fd = sd;
	fds.events = POLLIN;
	ret = poll(&fds, 1, msec);
	if (ret == -1) {
		perror("poll");
		ret = 0;
	}
	return ret;
}

static Shared_use_st* get_addr()
{
    void *shm = NULL;//分配的共享内存的原始首地址  
    int shmid;//共享内存标识符  

    shmid = shmget((key_t)KEY_NUM, sizeof(Shared_use_st), 0666|IPC_CREAT);
    if (shmid < 0) {perror("shmget");return NULL;}

    shm = shmat(shmid, 0, 0);
    if(shm == (void*)-1){perror("shmat");return NULL;};

    return (struct shared_use_st*)shm;
}

static void untach_addr(Shared_use_st* addr)
{
    if(shmdt((void*)addr) == -1)  
    {
	perror("shmdt");
    }

//    if(shmctl(shmid, IPC_RMID, 0) == -1)  
 //   {  
  //      fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
   //     exit(EXIT_FAILURE);  
    //} 
}

static int  copy_from_share_mem(Shared_use_st* st)
{
    struct shared_use_st *shared;//指向shm  
    shared=get_addr();
    if (shared == NULL) return -1;
    
    memcpy((void*)st,(void*)shared,sizeof(Shared_use_st));

    untach_addr(shared);
    return 0;
}
static int copy_to_share_mem(const char* addr,int num)
{
    struct shared_use_st *shared;//指向shm  
    shared=get_addr();
    if (shared == NULL) return -1;

    if (num > MAX_SIZE-1) num = MAX_SIZE-1;

    shared->size=num;
    memcpy(shared->text,addr,num);
    shared->text[num-1]='\0';
    printf("\033[32m""%s\n""\033[m", shared->text);

    untach_addr(shared);

    return num;
}

static int write_to_mem()
{
    int n;
    char buf[MAX_SIZE];

    n=read(STDIN_FILENO,buf,sizeof(buf));
    if (n <0){
	perror("read");
	return -1;
    }

    return copy_to_share_mem(buf,n);
}

static void copy_to_clip_baord(const char* str)
{
    char cmd[1024];
    printf("\033[35m""%s:copyed\n""\033[m",str);
    snprintf(cmd,sizeof(cmd),"echo \"%s\" | xclip -selection clipboard",str);
    system(cmd);
}

static int read_from_mem(const int line)
{
    int i,j;
    Shared_use_st st;

    if (copy_from_share_mem(&st)) return -1;

    char* buf=st.text;
    char read_buf[1024];
    int len = strlen(buf);
    int start=0;
    int end=0;
    int cur_line=0;

    for (i=0;i<len;i++)
    {
	if (buf[i] == '\n' || buf[i+1] == '\0'){
	    cur_line++;
	    end = i==len-1?i:i-1;
	    if (cur_line != line){
		start = i+1;
	    }else break;
	}
    }

    if (cur_line == line){
	if (end-start > sizeof(read_buf)-2) return 0;

	memcpy(read_buf,buf+start,end-start+1);
	read_buf[end-start+1]='\0';
	copy_to_clip_baord(read_buf);
    }
    return 0;
}



int main(int argc,char* argv[])
{
    int isHaveData=0;
    
    
    if (pollRead(STDIN_FILENO,1) > 0){
	isHaveData=1;
	write_to_mem();
     }


    if ( !isHaveData && argc == 1){
	read_from_mem(1);
	return 0;
    }

    if (argc > 1){
	read_from_mem(atoi(argv[1]));
    }

    return 0;
}
