#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/regs-clock.h>
#include <plat/regs-timer.h>
	 
#include <mach/regs-gpio.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define DEVICE_NAME "pin-abc"

static volatile unsigned int* Pin1;

static void print_reg(const char* name,int addr)
{
	Pin1=ioremap(addr,4);
	printk("%s 0x%x:0x%x\n",name,addr,*Pin1);
	iounmap(Pin1);
}

static void write_reg(const char* name,int addr,int val)
{
	Pin1=ioremap(addr,4);
	printk("write %s 0x%x:0x%x=0x%x\n",name,addr,*Pin1,val);
	*Pin1 = val;
	iounmap(Pin1);
}


typedef struct write_a{
        int ops;
        char name[20];
        int  addr;
        int val;
}Write_a;

static ssize_t fs_write (struct file * a, const char * data, size_t size, loff_t * b)
{
	unsigned long copy=0;
	char buf[1024];
	Write_a* wrA=(Write_a*)buf;
	copy = copy_from_user(buf, data, size); 
	if (copy){
	  printk("copy failed!\n");
	  return -1;
	}
	if (size != sizeof(Write_a)){
	  printk("size err size:%d\n",size);
	  return -1;
	}

	if (wrA->ops == 0)
	  print_reg(wrA->name,wrA->addr);
	else  if (wrA->ops == 1)
	  write_reg(wrA->name,wrA->addr,wrA->val);

	return size;
}

ssize_t fs_read(struct file *file, char __user *user, size_t size, loff_t * loff)
{
	Write_a wA;
	if (copy_to_user(user,&wA,sizeof(wA))){
		return -1;
	}
	return sizeof(wA);
}

static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.write = fs_write,
	.read = fs_read,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

#if 0
static irqreturn_t s3c_keypad_isr(int irq, void *dev_id)
{
	printk("irq happend!\n");
	return IRQ_HANDLED;
}
#endif

static int __init dev_init(void)
{
	int ret =0;
	printk (DEVICE_NAME" initialized\n");
	ret=misc_register(&misc);
	return ret;
}

static void __exit dev_exit(void)
{
	printk(DEVICE_NAME" removed\n");
	misc_deregister(&misc);
}
module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
