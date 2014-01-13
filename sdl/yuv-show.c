#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <signal.h>

#include "yuv-show.h"

static int isquit =0;
void catch_sig(int sig)
{
	isquit =1;
}
/* The pre-defined attributes of the screen */
// CIF
#define A8_SCREEN_WIDTH 800
#define A8_SCREEN_HEIGHT 480

int SCREEN_WIDTH  = 320;
int SCREEN_HEIGHT = 240;
const int SCREEN_BPP    = 8;

typedef struct yuv_pic
{
  char* pv_data[4];
  int   v_linesize[4]; //number of bytes per line
} s_picture;


SDL_Surface* ps_screen;
SDL_Overlay* ps_bmp;
SDL_Rect     s_rect;
char*        pv_frame = NULL;

int my_sdl_start(int ww,int hh)
{

  SCREEN_WIDTH =ww;
  SCREEN_HEIGHT =hh;
  int          v_pic_size = 0, v_frame_size = 0;
  s_picture    s_yuv_pic;

  if (signal(SIGINT,catch_sig)){
	perror("signal");
	return -1;
  }
  /* Init SDL */
  if( SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
    exit(1);
  }

  /* Set up the screen */
  //ps_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_SWSURFACE);
  ps_screen = SDL_SetVideoMode(A8_SCREEN_WIDTH, A8_SCREEN_HEIGHT, 0, SDL_SWSURFACE);
  if( !ps_screen )
  {
    fprintf(stderr, "SDL: could not set video mode -exiting\n");
    exit(1);
  }

  /* Set the window caption */
  SDL_WM_SetCaption( "YUV Window", NULL);

  /* Create a YUV overlay */
  ps_bmp = SDL_CreateYUVOverlay(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_YV12_OVERLAY, ps_screen);


  v_frame_size = (SCREEN_WIDTH * SCREEN_HEIGHT);
  v_pic_size   = ((v_frame_size * 3) / 2);
  if( (pv_frame = (char*)calloc(v_pic_size, 1)) == NULL )
  {
    fprintf(stderr, "SYS: could not allocate mem space\n");
    exit(2);
  }

  s_yuv_pic.pv_data[0] = pv_frame;  //Y
  s_yuv_pic.pv_data[1] = (char*)(pv_frame + v_frame_size);
  s_yuv_pic.pv_data[2] = (char*)(pv_frame + v_frame_size + v_frame_size/4);
  s_yuv_pic.v_linesize[0] = SCREEN_WIDTH;
  s_yuv_pic.v_linesize[1] = (SCREEN_WIDTH/2);
  s_yuv_pic.v_linesize[2] = (SCREEN_WIDTH/2);

  /* Apply the image to the screen */
  ps_bmp->pixels[0] = (unsigned char*)(s_yuv_pic.pv_data[0]);
  ps_bmp->pixels[2] = (unsigned char*)(s_yuv_pic.pv_data[1]);
  ps_bmp->pixels[1] = (unsigned char*)(s_yuv_pic.pv_data[2]);

  ps_bmp->pitches[0] = s_yuv_pic.v_linesize[0];
  ps_bmp->pitches[2] = s_yuv_pic.v_linesize[1];
  ps_bmp->pitches[1] = s_yuv_pic.v_linesize[2];

  /* Update the screen */
  s_rect.x = 0;
  s_rect.y = 0;
  s_rect.w = SCREEN_WIDTH+8;
  s_rect.h = SCREEN_HEIGHT+8;

	#if 0
	int sd=socket(AF_INET,SOCK_DGRAM,0);
        if (sd <0) {
        perror("socket");
        return -1;
        }

        struct sockaddr_in addr;

        addr.sin_family =AF_INET;
        addr.sin_port = htons(3303);
        addr.sin_addr.s_addr =INADDR_ANY;

        if (bind(sd,(struct sockaddr*)&addr,sizeof(addr))){
        perror("bind");
        return -1;
        }

        int re_use =1;
        if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&re_use,sizeof(re_use))){
        perror("setsockopt");
        return -1;
        }

	char* mallocbuf;
        mallocbuf=(char*)malloc(640*480*3/2);
        if (mallocbuf == NULL){
        perror("malloc");
        return -1;
        }
	#endif
  /* Load YUV frames */
  
	#if 0
	while(1)
        {
        char buf[40960];
	printf("before read\n");
	if (isquit) break;
        if ( read(sd,buf,sizeof(buf)) <0){
                perror("read");
        }
	if (isquit) break;
        const int len=*((int*)buf);
        printf("recv len:%d\n",len);
        int totallen;
        totallen=0;
        while(totallen != len){
        int recvlen=read(sd,buf,sizeof(buf));
	if (isquit) break;
        memcpy(mallocbuf+totallen,buf,recvlen);
        totallen+=recvlen;
	printf("totallen:%d\n",totallen);
        }
	memcpy(pv_frame,mallocbuf,len);
	SDL_LockYUVOverlay(ps_bmp);
	SDL_DisplayYUVOverlay(ps_bmp, &s_rect);
	SDL_UnlockYUVOverlay(ps_bmp);
        };break;
	#endif

//	printf("%02d:%06d framt at:%d\n",tv.tv_sec%60,tv.tv_usec,i);

   
	#if 0
	{
int sd 
	};break;
	#endif
 
   // SDL_Delay( 5 );

  return 0;
}

int one_sec_frames=0;
int total_frames =0;
int cur_sec =0;

int my_sdl_render(void* data)
{
	int i = 0, ret=0;
	memcpy(pv_frame,data,SCREEN_WIDTH*SCREEN_HEIGHT*3/2);
	one_sec_frames ++;
	total_frames++;
	i++;
	struct timeval tv;
	gettimeofday (&tv , NULL);
	if (cur_sec != tv.tv_sec%60){
		printf("cur_sec:%d,frames:%d\n",cur_sec,one_sec_frames);
		cur_sec = tv.tv_sec%60;
		one_sec_frames =0;
	}
	SDL_LockYUVOverlay(ps_bmp);
	SDL_DisplayYUVOverlay(ps_bmp, &s_rect);
	SDL_UnlockYUVOverlay(ps_bmp);
}

void my_sdl_close()
{
  /* Free the surfaces */
  free(pv_frame);

  /* Quit SDL */
  SDL_Quit();
}

#if 0
int main()
{
	FILE* fd = fopen("veggie.264.decode.yuv","rb");
	if (fd == NULL){
		perror("fopen");
		return -1;
	}
	const int w=320,h=240;
	my_sdl_start(w,h);
	char readbuf[1024*1024*3/2];
	while(!feof(fd))
	{
		fread(readbuf,1,w*h*3/2,fd);
		my_sdl_render(readbuf);
	}
	my_sdl_close();
	fclose(fd);
	return 0;
}
#endif
