static void spi_cmd(unsigned short cmd)
{
	Write_com(cmd);
}
static void spi_data(unsigned short val)
{
	Write_register(val);
}

static int gCmdLen =0;
static int gCurNum=0;
static char gBuf[100];

static void GP_COMMAD_PA(int len)
{
	int i;
	write_register(0xb7,0x0302);
        write_register(0xbd,0x0000);
        write_register(0xbc, len);

        spi_cmd(0xbf);
	gCmdLen=len;
	for (i=0;i<sizeof(gBuf);i++)	gBuf[i]=0x0;
}

static void SPI_WriteData(unsigned short val)
{
	int i;

	gBuf[gCurNum]=val;
	gCurNum++;

	if (gCurNum == gCmdLen){
	    gCurNum=0;

	    for (i=0;i<gCmdLen;i+=2)
	    {
		Write_register(((gBuf[i+1]<<8)&0xff00)+(gBuf[i+0]&0xff));
	    }
	    #if 0
	    printf("len:%d ",gCmdLen);    
	    for (i=0;i<gCmdLen;i++)
	    {
		printf("data[%d]:0x%X ",i,gBuf[i+1]&0xff);
	    }
	    printf("\n");
	    #endif
	}
}

static U16 GP_COMMAD_PA_READ()
{
        U32 i,count;
        SPI_2825_WrReg(0xB7,0x382);
        SPI_2825_WrReg(0xBC,0x01);
        SPI_2825_WrReg(0xBF,0x0a);

        Set_2805_CS(1);
        Delay_ms(100);
        printf("C6:0x%X\n",SPI_READ_ID(0xC6));
        if ((SPI_READ_ID(0xC6)&0x19) !=0x19){
           printf("read 0xC6 not ready failed C3:0x%X\n",SPI_READ_ID(0xC3));
           return 0xFFFF;
        }

        count = SPI_READ_ID(0xC2);
        printf("readnum:0x%X ",count);
        if (count >100) count =100;
        for (i=0;i<count;i++)
        {
            printf("data[%d]:0x%X ",i,SPI_READ_ID(0xFF));
        } printf("\n");
	return 0xFFFF;
//      return SPI_READ_ID(cmd);
}


static void origin_factory_init()
{
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(4);SPI_WriteData(0xff);SPI_WriteData(0x12);SPI_WriteData(0x83);SPI_WriteData(0x01);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(3);SPI_WriteData(0xff);SPI_WriteData(0x12);SPI_WriteData(0x83);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x92);
GP_COMMAD_PA(3);SPI_WriteData(0xff);SPI_WriteData(0x30);SPI_WriteData(0x02);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(8);SPI_WriteData(0xc0);SPI_WriteData(0x00);SPI_WriteData(0x64);SPI_WriteData(0x00);SPI_WriteData(0x10);SPI_WriteData(0x10);SPI_WriteData(0x00);SPI_WriteData(0x64);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(3);SPI_WriteData(0xc0);SPI_WriteData(0x10);SPI_WriteData(0x10);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(7);SPI_WriteData(0xc0);SPI_WriteData(0x00);SPI_WriteData(0x5c);SPI_WriteData(0x00);SPI_WriteData(0x01);SPI_WriteData(0x00);SPI_WriteData(0x04);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa2);
GP_COMMAD_PA(2);SPI_WriteData(0xc0);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa3);
GP_COMMAD_PA(2);SPI_WriteData(0xc0);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa4);
GP_COMMAD_PA(2);SPI_WriteData(0xc0);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(2);SPI_WriteData(0xc4);SPI_WriteData(0x18);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb3);
GP_COMMAD_PA(3);SPI_WriteData(0xc0);SPI_WriteData(0x00);SPI_WriteData(0x50);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x81);
GP_COMMAD_PA(2);SPI_WriteData(0xc1);SPI_WriteData(0x55);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(2);SPI_WriteData(0xc4);SPI_WriteData(0x49);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x36);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(8);SPI_WriteData(0xc4);SPI_WriteData(0x05);SPI_WriteData(0x10);SPI_WriteData(0x06);SPI_WriteData(0x02);SPI_WriteData(0x05);SPI_WriteData(0x15);SPI_WriteData(0x10);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa7);
GP_COMMAD_PA(8);SPI_WriteData(0xc4);SPI_WriteData(0x05);SPI_WriteData(0x10);SPI_WriteData(0x07);SPI_WriteData(0x02);SPI_WriteData(0x05);SPI_WriteData(0x15);SPI_WriteData(0x10);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(3);SPI_WriteData(0xc4);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x91);
GP_COMMAD_PA(3);SPI_WriteData(0xc5);SPI_WriteData(0x46);SPI_WriteData(0x40);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(3);SPI_WriteData(0xd8);SPI_WriteData(0xcf);SPI_WriteData(0xcf);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xd9);SPI_WriteData(0x55);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x81);
GP_COMMAD_PA(2);SPI_WriteData(0xc4);SPI_WriteData(0x82);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(3);SPI_WriteData(0xc5);SPI_WriteData(0x04);SPI_WriteData(0xb8);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xbb);
GP_COMMAD_PA(2);SPI_WriteData(0xc5);SPI_WriteData(0x80);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x82);
GP_COMMAD_PA(2);SPI_WriteData(0xc4);SPI_WriteData(0x02);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc6);
GP_COMMAD_PA(2);SPI_WriteData(0xb0);SPI_WriteData(0x03);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xd0);SPI_WriteData(0x40);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(3);SPI_WriteData(0xd1);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(5);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x97);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x9e);
GP_COMMAD_PA(2);SPI_WriteData(0xcb);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa7);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xae);
GP_COMMAD_PA(2);SPI_WriteData(0xcb);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb7);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xbe);
GP_COMMAD_PA(2);SPI_WriteData(0xcb);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc7);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x00);SPI_WriteData(0x05);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xce);
GP_COMMAD_PA(2);SPI_WriteData(0xcb);SPI_WriteData(0x05);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd7);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xde);
GP_COMMAD_PA(2);SPI_WriteData(0xcb);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xe0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x00);SPI_WriteData(0x05);SPI_WriteData(0x05);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xe7);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xf0);
GP_COMMAD_PA(8);SPI_WriteData(0xcb);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xf7);
GP_COMMAD_PA(5);SPI_WriteData(0xcb);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x0e);SPI_WriteData(0x10);SPI_WriteData(0x0a);SPI_WriteData(0x0c);SPI_WriteData(0x02);SPI_WriteData(0x04);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x2e);SPI_WriteData(0x2d);SPI_WriteData(0x00);SPI_WriteData(0x29);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x8e);
GP_COMMAD_PA(2);SPI_WriteData(0xcc);SPI_WriteData(0x2a);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x97);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x0d);SPI_WriteData(0x0f);SPI_WriteData(0x09);SPI_WriteData(0x0b);SPI_WriteData(0x01);SPI_WriteData(0x03);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x9e);
GP_COMMAD_PA(2);SPI_WriteData(0xcc);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x2e);SPI_WriteData(0x2d);SPI_WriteData(0x00);SPI_WriteData(0x29);SPI_WriteData(0x2a);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa7);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x0b);SPI_WriteData(0x09);SPI_WriteData(0x0f);SPI_WriteData(0x0d);SPI_WriteData(0x03);SPI_WriteData(0x01);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb7);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x2d);SPI_WriteData(0x2e);SPI_WriteData(0x00);SPI_WriteData(0x29);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xbe);
GP_COMMAD_PA(2);SPI_WriteData(0xcc);SPI_WriteData(0x2a);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc0);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc7);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x0c);SPI_WriteData(0x0a);SPI_WriteData(0x10);SPI_WriteData(0x0e);SPI_WriteData(0x04);SPI_WriteData(0x02);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xce);
GP_COMMAD_PA(2);SPI_WriteData(0xcc);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd0);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x2d);SPI_WriteData(0x2e);SPI_WriteData(0x00);SPI_WriteData(0x29);SPI_WriteData(0x2a);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd7);
GP_COMMAD_PA(8);SPI_WriteData(0xcc);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x8b);SPI_WriteData(0x03);SPI_WriteData(0x18);SPI_WriteData(0x8a);SPI_WriteData(0x03);SPI_WriteData(0x18);SPI_WriteData(0x89);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(6);SPI_WriteData(0xce);SPI_WriteData(0x03);SPI_WriteData(0x18);SPI_WriteData(0x88);SPI_WriteData(0x03);SPI_WriteData(0x18);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x97);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x07);SPI_WriteData(0x05);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa7);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x06);SPI_WriteData(0x05);SPI_WriteData(0x01);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x02);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb7);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x04);SPI_WriteData(0x05);SPI_WriteData(0x03);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc0);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x03);SPI_WriteData(0x05);SPI_WriteData(0x04);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc7);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x02);SPI_WriteData(0x05);SPI_WriteData(0x05);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd0);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x01);SPI_WriteData(0x05);SPI_WriteData(0x06);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xd7);
GP_COMMAD_PA(8);SPI_WriteData(0xce);SPI_WriteData(0x38);SPI_WriteData(0x00);SPI_WriteData(0x05);SPI_WriteData(0x07);SPI_WriteData(0x00);SPI_WriteData(0x18);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x80);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x87);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x97);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa7);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb0);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb7);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc0);
GP_COMMAD_PA(8);SPI_WriteData(0xcf);SPI_WriteData(0x3d);SPI_WriteData(0x02);SPI_WriteData(0x15);SPI_WriteData(0x20);SPI_WriteData(0x00);SPI_WriteData(0x00);SPI_WriteData(0x01);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xc7);
GP_COMMAD_PA(5);SPI_WriteData(0xcf);SPI_WriteData(0x81);SPI_WriteData(0x00);SPI_WriteData(0x03);SPI_WriteData(0x08);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb5);
GP_COMMAD_PA(7);SPI_WriteData(0xc5);SPI_WriteData(0x00);SPI_WriteData(0x6f);SPI_WriteData(0xff);SPI_WriteData(0x00);SPI_WriteData(0x6f);SPI_WriteData(0xff);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(5);SPI_WriteData(0xf5);SPI_WriteData(0x02);SPI_WriteData(0x11);SPI_WriteData(0x02);SPI_WriteData(0x11);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x90);
GP_COMMAD_PA(2);SPI_WriteData(0xc5);SPI_WriteData(0x50);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x94);
GP_COMMAD_PA(2);SPI_WriteData(0xc5);SPI_WriteData(0x66);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb2);
GP_COMMAD_PA(3);SPI_WriteData(0xf5);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb4);
GP_COMMAD_PA(3);SPI_WriteData(0xf5);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb6);
GP_COMMAD_PA(3);SPI_WriteData(0xf5);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb8);
GP_COMMAD_PA(3);SPI_WriteData(0xf5);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x94);
GP_COMMAD_PA(2);SPI_WriteData(0xf5);SPI_WriteData(0x02);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xba);
GP_COMMAD_PA(2);SPI_WriteData(0xf5);SPI_WriteData(0x03);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xb4);
GP_COMMAD_PA(2);SPI_WriteData(0xc5);SPI_WriteData(0xc0);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x17);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x1c);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x08);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0b);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x09);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x04);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x07);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0e);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x08);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x13);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x0d);
GP_COMMAD_PA(2);SPI_WriteData(0xe1);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x17);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x1c);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x08);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0b);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x09);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x04);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x07);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0e);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x08);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0f);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x13);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x0d);
GP_COMMAD_PA(2);SPI_WriteData(0xe2);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0xfb);SPI_WriteData(0x01);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x35);SPI_WriteData(0x00);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0xa0);
GP_COMMAD_PA(2);SPI_WriteData(0xc1);SPI_WriteData(0x02);
GP_COMMAD_PA(2);SPI_WriteData(0x00);SPI_WriteData(0x00);
GP_COMMAD_PA(4);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);SPI_WriteData(0xff);
GP_COMMAD_PA(1);SPI_WriteData(0x11);
Delay_ms(120);
GP_COMMAD_PA(1);SPI_WriteData(0x29);
}

