#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/cdev.h>

#include <linux/device.h>
#include <linux/i2c/at24.h>

#define AT24XX_NAME	"at24c04"
#define MAX_LEN	128

static int major;
static struct class *cls;
struct i2c_client *at24cxx_client;

static int at24xx_read(struct file *file, char __user *buf, size_t size, loff_t * offset)
{	
	unsigned char address;
	unsigned char data[MAX_LEN];
	unsigned int len;
	struct i2c_msg msg[2];
	int ret;
	
	len = (size>MAX_LEN)? MAX_LEN:size;
	
	copy_from_user(&address, buf, len);

	/* 数据传输三要素: 源,目的,长度 */

	/* 读AT24CXX时,要先把要读的存储空间的地址发给它 */
	msg[0].addr  = at24cxx_client->addr;  /* 目的 */
	msg[0].buf   = &address;              /* 源 */
	msg[0].len   = 1;                     /* 地址=1 byte */
	msg[0].flags = 0;                     /* 表示写 */

	/* 然后启动读操作 */
	msg[1].addr  = at24cxx_client->addr;  /* 源 */
	msg[1].buf   = &data;                 /* 目的 */
	msg[1].len   = len;                     /* 数据=1 byte len here*/
	msg[1].flags = I2C_M_RD;                     /* 表示读 */

	ret = i2c_transfer(at24cxx_client->adapter, msg, len);
	if (ret < 0){
		pr_err("%s: i2c read error: %d\n", __func__, ret);
		return -EIO;
	}
	copy_to_user(buf, &data, ret);
	return ret;
}

static ssize_t at24xx_write(struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	unsigned char ker_buf[2];
	unsigned char addr, data;
	
	copy_from_user(ker_buf, buf, 2);
	addr = ker_buf[0];
	data = ker_buf[1];
	
	if (!i2c_smbus_write_byte_data(at24cxx_client, addr, data))
		return 2;
	else
		return -EIO;
}
	
static struct file_operations at24xx_fops = {
	.owner	= THIS_MODULE,
	.read	= at24xx_read,
	.write	= at24xx_write,
};

static int at24xx_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	at24cxx_client = client;
	printk("%s %s %d \n",__FILE__, __func__, __LINE__);
	major = register_chrdev(0,"at24xx",&at24xx_fops);
	cls = class_create(THIS_MODULE, "at24xx");
	device_create(cls, NULL, MKDEV(major,0), NULL, "at24xx");
	return 0;
}

static int __devexit at24xx_remove(struct i2c_client *client) {
	struct at24xx_data *at = i2c_get_clientdata(client);
printk("%s %s %d \n",__FILE__, __func__, __LINE__);
	i2c_set_clientdata(client, NULL);
	
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major,"at24xx");
	
	return 0;
}

static int at24xx_detect(struct i2c_client *client, struct i2c_board_info *info) {
	printk("at24xx detect : addr = 0x%x\n", client->addr);
	if (client->addr == 0x50)
		return 0;
	else
	return -ENODEV;
}

static const struct i2c_device_id at24xx_id[] = {
	{ AT24XX_NAME, 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, at24xx_id);

static struct i2c_driver at24xx_driver = {
	.probe		= at24xx_probe,
	.remove		= __devexit_p(at24xx_remove),
	.id_table	= at24xx_id,
//	.detect		= at24xx_detect,
	.driver	= {
		.name	= "at24xx",
		.owner	= THIS_MODULE,
	},
};

static int __init at24xx_init(void)
{
	return i2c_add_driver(&at24xx_driver);
}

static void __exit at24xx_exit(void)
{
	i2c_del_driver(&at24xx_driver);
}

module_init(at24xx_init);
module_exit(at24xx_exit);

MODULE_AUTHOR("<yk@blindgan.com>");
MODULE_DESCRIPTION("at24xx driver");
MODULE_LICENSE("GPL");
