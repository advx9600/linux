obj-m:=s3c-keypad.o
#obj-m:=pin_abc.o
KDIR:=/home/abc/Public/linux/s5pv210_self/linux-2.6.32-Realv210
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers
