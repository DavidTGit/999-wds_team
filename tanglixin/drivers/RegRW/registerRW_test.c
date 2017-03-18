#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

static struct class *my_class;
static struct class_device  *my_class_dev;
int major;

static int my_drv_read(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
    unsigned long val[2];//val[0] store the register address, val[1] store the register value
    volatile unsigned long * reg_add = NULL;//register address

    copy_from_user(&val, buf, 8);//get register address val[0]

    reg_add =( volatile unsigned long * )(ioremap(val[0], 4);//cast register address to a volatile pointer
    val[1] = (unsigned long)*reg_add;

    copy_to_user( buf,&val, 8);
    iounmap(reg_add);

    return 0;
}

static ssize_t my_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
    unsigned long val[2];//val[0] store the register address, val[1] store the register value
    volatile unsigned long * reg_add = NULL;

    copy_from_user(&val, buf, 8); 

    reg_add =( volatile unsigned long * )(ioremap(val[0], 4);
    *reg_add = val[1];
    iounmap(reg_add);

    return 0;
}

static struct file_operations my_drv_fops = {
    .owner  =   THIS_MODULE,
    .read   =   my_drv_read,
    .write  =   my_drv_write,      
};

static int my_drv_init(void)
{
    major = register_chrdev(0, "regrw_drv", &my_drv_fops);
    my_class = class_create(THIS_MODULE, "regrwdrv");
    my_class_dev = class_device_create(my_class, NULL, MKDEV(major, 0), NULL, "reg"); /* ´´½¨file node: /dev/reg */

    return 0;
}

static void my_drv_exit(void)
{
    unregister_chrdev(major, "regrw_drv"); 
    class_device_unregister(my_class_dev);
    class_destroy(my_class);
}

module_init(my_drv_init);
module_exit(my_drv_exit);


MODULE_LICENSE("GPL");

