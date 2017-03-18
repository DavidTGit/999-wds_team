#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>

static ssize_t mymsg_read(struct file *f, char __user *buf, 
			size_t count, loff_t *ppos) {
	printk("mymsg_read\n");
	return 0;
}

static ssize_t mymsg_write(struct file *f, char __user *buf, 
			size_t count, loff_t *ppos) {
	printk("mymsg_write\n");
	return 0;
}

const struct file_operations proc_mymsg_ops = {
	.read = mymsg_read,
	.write = mymsg_write
};

static int mymsg_init(void){
	struct proc_dir_entry *entry;
	int rc = -ENOMEM;
	entry = proc_create("mymsg", S_IRUSR, NULL,&proc_mymsg_ops);
	if(!entry)
		goto out_interface;
	rc = 0;
	
out:	return rc;

out_interface:
	remove_proc_entry("mymsg", NULL);
}

static void mymsg_exit(void){
	remove_proc_entry("mymsg", NULL);
}

module_init(mymsg_init);
module_exit(mymsg_exit);

MODULE_LICENSE("GPL");
//EXPORT("mymsg");

