#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
#include <fcntl.h>

# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include "mad.h"

struct buffer {
  unsigned char const *start;
  unsigned long length;
};

static FILE* gFd;

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static
enum mad_flow input(void *data,
                    struct mad_stream *stream)
{
  struct buffer *buffer = data;

  if (!buffer->length)
    return MAD_FLOW_STOP;

  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

static
enum mad_flow output(void *data,
                     struct mad_header const *header,
                     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  short bufShort[nsamples];
  int i=0;

  while (nsamples--) {
    signed int sample;

    /* output sample(s) in 16-bit signed little-endian PCM */

    sample = scale(*left_ch++);
    bufShort[i++] = sample;
    #if 0
    putchar((sample >> 0) & 0xff);
    putchar((sample >> 8) & 0xff);
    #endif

    #if 0
    if (nchannels == 2) {
      sample = scale(*right_ch++);
      putchar((sample >> 0) & 0xff);
      putchar((sample >> 8) & 0xff);
    }
    #endif
  }
  fwrite(bufShort,2,i,gFd);

  return MAD_FLOW_CONTINUE;
}

static
enum mad_flow error(void *data,
                    struct mad_stream *stream,
                    struct mad_frame *frame)
{
  #if 0
  struct buffer *buffer = data;

  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
          stream->error, mad_stream_errorstr(stream),
          (unsigned int)(stream->this_frame - buffer->start));
  #endif

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

static
int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;

  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */

  mad_decoder_init(&decoder, &buffer,
                   input, 0 /* header */, 0 /* filter */, output,
                   error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&decoder);
}

int main(int argc, char * argv[])
{
  struct stat stat;
  void *fdm;

  if (argc != 3){
    printf("argc err\ninput mp3 filename,output pcm filename\n");
    return -1;
  }
  const char* mp3File = argv[1];
  const char* pcmFile = argv[2];

  int fd = open(mp3File,O_RDONLY);
  if (fd < 0) {perror("open"); return -1; }

  gFd = fopen(pcmFile,"wb");
  if (gFd == NULL) {perror("fopen");return -1;}

  if (fstat(fd, &stat) == -1 ||
      stat.st_size == 0)
    return 2;

  fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (fdm == MAP_FAILED)
    return 3;

  decode(fdm, stat.st_size);

  if (munmap(fdm, stat.st_size) == -1)
    return 4;

  fclose(gFd);

  close(fd);

  return 0;
}
