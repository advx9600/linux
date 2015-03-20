/************************************************************/
/****改变MIPI通道的，SSD2825需要修改参数的寄存器0xDE,0xBA****/
/****      0xDE = n-1 (n为你想要的MIPI通道数)            ****/
/****      0xBA 设置详见SSD2828规格书                    ****/
/************************************************************/
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define VBP                     (0xe)
#define LCD_VBPD                VBP
#define VFP                     (0x0a)
#define LCD_VFPD                VFP
#define VPW                     (2)
#define LCD_VSPW                VPW

#define HBP                     (0x56)
#define LCD_HBPD                HBP
#define HFP                     (0x52)
#define LCD_HFPD                HFP
#define HPW                     (4)
#define LCD_HSPW                HPW

#define LCD_XSIZE_TFT           (720)
#define LCD_YSIZE_TFT           (1280)
#define LCD_WIDTH               LCD_XSIZE_TFT
#define LCD_HEIGHT              LCD_YSIZE_TFT

// clk 24M
#define LCD_DSI_CLCK_LOW                (0x4214)
#define LCD_DSI_CLCK_NORMAL             (0x822b)

#define MDELAY Delay_ms
#define SET_RESET_PIN   Set_RST
#define Read_Register   SPI_READ_ID


static __inline unsigned short write_spi_data(unsigned char addr,unsigned short data)
{
	char buf[3];
	buf[0]=addr;
	buf[1]= (data>>8);
	buf[2]= data;
	if ((addr&0xff) == 0x73){
	    #if 1
	    char readbuf[3];
	    send_data_origin(gFd,buf,readbuf,3);
//	    printf("0x%X 0x%X 0x%X\n",readbuf[0],readbuf[1],readbuf[2]);
	    return (readbuf[1]<<8)+readbuf[2]&0xffff;
	    #endif
	}
	write(gFd,buf,3);
	return 0;
}
static __inline void Write_com(unsigned short cmd) //write cmd
{	
	write_spi_data(0x70,cmd);
}

static __inline void Write_register(unsigned short data)
{	
	write_spi_data(0x72,data);
}

static void write_register(unsigned short reg,unsigned short val)
{
        Write_com(reg);
        Write_register(val);
}
static __inline unsigned short Read_Register(unsigned short reg)
{	
	Write_com(reg);
	return write_spi_data(0x73,0x0);
}

static void write_com_reg(char cmd,U16 reg)
{
	Write_com(cmd);
	Write_register(reg);
}

static void lcm_config(void)
{
	write_com_reg(0xB7,0x0050);
	write_com_reg(0xB8,0x0000);
	write_com_reg(0xB9,0x0000);
	write_com_reg(0xBA,0x4214);
	write_com_reg(0xBB,0x0003);
	write_com_reg(0xB9,0x0001);
	write_com_reg(0xDE,0x0003);
	write_com_reg(0xC9,0x2302);
	write_com_reg(0xB7,0x0050);
	write_com_reg(0xB8,0x0000);
	write_com_reg(0xB9,0x0000);
	write_com_reg(0xBA,0x822b);
	write_com_reg(0xBB,0x0007);
	write_com_reg(0xB9,0x0001);
	write_com_reg(0xC9,0x2302);
	usleep(5);
	write_com_reg(0xCA,0x2301);
	write_com_reg(0xCB,0x0510);
	write_com_reg(0xCC,0x1005);
	usleep(5);
	write_com_reg(0xD0,0x0000);
	usleep(5);
}

static void lcm_init(void)
{
	write_com_reg(0xb1,(LCD_VSPW<<8)|LCD_HSPW);
	write_com_reg(0xb2,(LCD_VBPD<<8)|LCD_HBPD);
	write_com_reg(0xb3,(LCD_VFPD<<8)|LCD_HFPD);
	write_com_reg(0xb4, LCD_XSIZE_TFT);
	write_com_reg(0xb5, LCD_YSIZE_TFT);
	write_com_reg(0xb6, 0x0003);

	write_com_reg(0xDE,0x0003);
	write_com_reg(0xD6,0x0004);
	write_com_reg(0xB7,0x024b);
	usleep(100);

	Write_com(0x2C);
}
