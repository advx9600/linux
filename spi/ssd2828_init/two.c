#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SPI_NO_CS 0x40 /* 1 dev/bus, no chipselect */
#define SPI_READY 0x80

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev1.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;


static void send_data_origin(int fd,char* tx,char* rx,int len)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret == 1)
		pabort("can't send spi message");
}

static int send_count=0;
static void transfer(int fd)
{
   char tx[]={0x80,0x00};
   int len = sizeof(tx);
   char rx[len];
   int i;
   for (i=0;i<len;i++){
	 rx[i]=0;
   }
   
   send_count++;
   send_data_origin(fd,tx,rx,len);
   printf("%02d ",send_count);
   for (i=0;i<len;i++)
	printf("rx[%d]:0x%X ",i,rx[i]&0xFF);
   printf("\n");
}

static int gFd;
#define U32	unsigned int
#define U16	unsigned short
#define U8	unsigned char
#define Delay_ms(t) usleep(t*1000)

// for SSD2828-SPI-1.c
#define SPI_2825_WrReg	write_register
#define Set_2805_CS(t)  do{}while(0)
#define SPI_READ_ID(t) Read_Register(t)

//#include "ssd2828_pin.c"
#include "ssd2828_inti_reference_only.c"
#include "OT1287_HSD698_20150112.c"

int main()
{
	int ret = 0;
	int i;
	int fd;


	fd =gFd= open(device, O_RDWR);
	if (fd < 0){
		printf("%s\n",device);
		pabort("can't open device");
	}
//	mode |= SPI_CPHA;
     // mode |= SPI_CPOL;


	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

//	ssd2828_gpio_cfg();

	lcm_config();
	printf("2828 ID:0x%X\n",SPI_READ_ID(0xB0));

	origin_factory_init();
	GP_COMMAD_PA_READ();

	lcm_init();


	return 0;
/****
if OK should print like
spi mode: 0
bits per word: 8
max speed: 500000 Hz (500 KHz)
2828 ID:0x2828
C6:0xC79F
readnum:0x1 data[0]:0x9C
****/

}
