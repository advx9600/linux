/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

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

#define SPI_NO_CS       0x40                    /* 1 dev/bus, no chipselect */
#define SPI_READY       0x80

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
static int origin_level=0;

static int count=0;

static void set_data_bit_value(char* tx,int* txBit)
{
	int i;
	tx[0]=tx[1]=0;
	for (i=0;i<16;i++)
	{
		if (!txBit[i]) continue;
		int cur =i>7?1:0;
		tx[!cur] |=  (1<<(i-cur*8) & 0xff);
	}
}

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
static void set_max3100_config(int fd,int baudRate)
{
	int ret;
	int i;
	uint8_t tx[] = {
		0xFF, 0xFF,
	};
	int txBit[16];
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };

	txBit[15]=0x1;
	txBit[14]=0x1;
	txBit[13]=0x0; // 0 enable fifo,1 disable
	txBit[12]=0x0; // software shutdown
	txBit[11]=0x0;
	txBit[10]=0x1; 
	txBit[9]=txBit[8]=0x0; // mask// RM
	txBit[7]=0x0;// enable timing
	txBit[6]=0x0; // stop bit
	txBit[5]=0x0; // parity enable bit
	txBit[4]=0x0; // world length 8
	/* baud rate */
	txBit[3]=txBit[2]=txBit[1]=0x1; txBit[0]=0x1; // default 115200
	txBit[2]=0x0; // set 9600

	/* set baud rate */
	/*
	const int minSpeed1 = 1800;
	const int minSpeed2 = 600;
	if (bandRate%1800 == 0){
	  int num = bandRate/minSpeed1;
	  int po=0;
	  while(num/2 !== 1)
	  {
		po++;
	  }
	}else{
	}
	*/

	set_data_bit_value(tx,txBit);

	printf("%d  ",count);
	count++;
	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		printf("tx[%d]:0x%.2X ",ret,tx[ret]);
	}
	puts("");

	send_data_origin(fd,tx,NULL,2);
//	printf("0x%.2x,0x%.2x\n",rx[0],rx[1]);

	tx[0] &= 0x7F; // read data
	rx[0]=rx[1]=0;
	
	char temp=tx[1];
	tx[1]=0x0;
	send_data_origin(fd,tx,rx,2);
	tx[1]=temp;

	if (tx[0] != rx[0] || tx[1] != rx[1]){
		printf(".....\n");
		printf("tx[0]:0x%.2x,rx[0]:0x%.2x\n",tx[0],rx[0]);
		printf("tx[1]:0x%.2x,rx[1]:0x%.2x\n",tx[1],rx[1]);
		pabort("read and write is nocorrect");
	}
}

static void set_origin_level(int fd,int level)
{
	char tx[2]={0xff,0xff};
	int txBit[16];
	
	origin_level=level;

	txBit[15]=0x1;
	txBit[14]=txBit[13]=txBit[12]=txBit[11]=0x0;
	txBit[10]=0x1; // Transmit-Enable Bit for 485
	txBit[9] = !origin_level;// Request-to-Send Bit. Controls the state of the RTS output.
	txBit[8]=0x0; // parity

	set_data_bit_value(tx,txBit);
	tx[1]=0x0;
	send_data_origin(fd,tx,NULL,2);
}

static void send_data(int fd,char data)
{
	char tx[2]={0xff,0xff};
	int txBit[16];
	
	txBit[15]=0x1;
	txBit[14]=txBit[13]=txBit[12]=txBit[11]=0x0;
	txBit[10]=0x0; // Transmit-Enable Bit for 485
	txBit[9] = !origin_level;// Request-to-Send Bit. Controls the state of the RTS output.
	txBit[8]=0x0; // parity

	txBit[9]=!txBit[9];
	txBit[10]=0x1;
	set_data_bit_value(tx,txBit);
	tx[1]=0x0;
//	send_data_origin(fd,tx,NULL,2);

	txBit[9]=!txBit[9];
	txBit[10]=0x0;
	set_data_bit_value(tx,txBit);
	tx[1]=data;
	send_data_origin(fd,tx,NULL,2);
	
	#if 0
	txBit[9]=!txBit[9];
	set_data_bit_value(tx,txBit);
	send_data_origin(fd,tx,NULL,2);
	#endif
}

static void recv_data(int fd,char* rx)
{
	char tx[2]={0x0,0x0};
	send_data_origin(fd,tx,rx,2);
	printf("receive 0x%.2x 0x%.2x\n",rx[0]&0xff,rx[1]&0xFF);
}


static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.0)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int i;
	int fd;

	parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0){
		printf("%s\n",device);
		pabort("can't open device");
	}
//	mode |= SPI_CPHA;
 //     mode |= SPI_CPOL;


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

	set_origin_level(fd,0); // 1 send ,0 recv

	set_max3100_config(fd,0); // baud rate set not affect
	
	#if 1
	printf("only set config\n");
	return 0;
	#endif

	#if 1 // write loop
	int send_times=0;
	while(1)
	{
		sleep(1);
		send_times++;
		printf("send times:%d\n",send_times);
		// for (i=0;i<10;i++)
		//	send_data(fd,'a'+i);
		char cc=0x55;//0x61;//97
		send_data(fd,cc);
	}
	#endif
	
	#if 1 // recv loop
	int recv_count=0;
	while(1)
	{
		sleep(1);
		char recv_buf[2];
		recv_count++;
		recv_data(fd,recv_buf);	
		printf("recv times:%d\n",recv_count);
	}
	#endif

	close(fd);

	return ret;
}
