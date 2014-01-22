

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/soundcard.h>

#define OPEN_DSP_FAILED     0x00000001    /*open dsp failed!*/
#define SAMPLERATE_STATUS     0x00000002    /*samplerate status failed*/
#define SET_SAMPLERATE_FAILED  0x00000003       /*set samplerate failed*/
#define CHANNELS_STATUS       0x00000004    /*Channels status failed*/
#define SET_CHANNELS_FAILED    0x00000005    /*set channels failed*/
#define FMT_STATUS       0x00000006    /*FMT status failed*/
#define SET_FMT_FAILED     0x00000007    /*set fmt failed*/
#define OPEN_FILE_FAILED        0x00000008    /*opem filed failed*/

int P8100_Audio_Play(char *pathname,int nSampleRate,int nChannels,int fmt)
{
 int dsp_fd,mix_fd,status,arg;
 dsp_fd = open("/dev/dsp" , O_RDWR);   /*open dsp*/
 if(dsp_fd < 0) 
 {
  return  OPEN_DSP_FAILED;
 }
 arg = nSampleRate;
 status = ioctl(dsp_fd,SOUND_PCM_WRITE_RATE,&arg); /*set samplerate*/
 if(status < 0)
 {
  close(dsp_fd);
  return SAMPLERATE_STATUS;
 } 
 if(arg != nSampleRate)
 {
  close(dsp_fd);
  return SET_SAMPLERATE_FAILED;
 } 
 arg = nChannels;  /*set channels*/   
 status = ioctl(dsp_fd, SOUND_PCM_WRITE_CHANNELS, &arg); 
 if(status < 0)
 {
  close(dsp_fd);
  return CHANNELS_STATUS;
 } 
 if( arg != nChannels)
 {
  close(dsp_fd);
  return SET_CHANNELS_FAILED;
 }
 arg = fmt; /*set bit fmt*/
 status = ioctl(dsp_fd, SOUND_PCM_WRITE_BITS, &arg); 
 if(status < 0)
 {
  close(dsp_fd);
  return FMT_STATUS;
 } 
 if(arg != fmt)
 {
  close(dsp_fd);
  return SET_FMT_FAILED;
 }/*到此设置好了DSP的各个参数*/             
 FILE *file_fd = fopen(pathname,"r");
 if(file_fd == NULL)
 {
  close(dsp_fd);
  return OPEN_FILE_FAILED;
 }
 int num = 3*nChannels*nSampleRate*fmt/8;
 int get_num; 
 char buf[num];
 while(feof(file_fd) == 0)
 {
  get_num = fread(buf,1,num,file_fd);
  write(dsp_fd,buf,get_num);
  if(get_num != num)
  {
   close(dsp_fd);
   fclose(file_fd);
   return 0;
  } 
 }
 close(dsp_fd);
 fclose(file_fd);
 return 0; 
}
/*
*test
*/
int main()
{
 int value;
 value = P8100_Audio_Play("a.pcm",8000,1,16);
 fprintf(stderr,"value is %d",value);
 return 0;
} 

