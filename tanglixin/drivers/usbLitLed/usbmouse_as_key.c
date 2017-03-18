
/*
 * drivers\hid\usbhid\usbmouse.c
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
/////////////////////////
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>

#include <asm/mach/map.h>
#include <asm/arch/regs-lcd.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/fb.h>


#define GPIO_BASE ((unsigned long)0x56000010)
#define CON_REG   GPIO_BASE
#define DATA_REG (CON_REG+4)

#define LED_ON(n) (~(1<<n))
#define LED_OFF(n) (1<<n)
#define EN_LED(n) (0x1<<(n*2))
#define LED_MSK(n) (0x3<<(n*2))

static struct led_dev
{
    volatile unsigned long* gpiocon;
    volatile unsigned long* gpiodata;

}led_dev;

static struct input_dev *uk_dev;
static char *usb_buf;
static dma_addr_t usb_buf_phys;
static int len;
static struct urb *uk_urb;
static char  namebuf[128];

static struct usb_device_id usbmouse_as_key_id_table [] = {
    { USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
        USB_INTERFACE_PROTOCOL_MOUSE) },
    //{USB_DEVICE(0x1234,0x5678)},
    { } /* Terminating entry */
};

static void usbmouse_as_key_irq(struct urb *urb)
{
    static unsigned char pre_val;
    /* USB鼠标数据含义
     * data[0]: bit0-左键, 1-按下, 0-松开
     *          bit1-右键, 1-按下, 0-松开
     *          bit2-中键, 1-按下, 0-松开 
     *
     */
    if ((pre_val & (1<<0)) != (usb_buf[1] & (1<<0)))
    {
        /* 左键发生了变化 */
        *(led_dev.gpiodata) &= LED_ON(5) & LED_ON(6) & LED_ON(7)& LED_ON(8);

    }

    if ((pre_val & (1<<1)) != (usb_buf[1] & (1<<1)))
    {
        /* 右键发生了变化 */
        *(led_dev.gpiodata) |= LED_OFF(5)|LED_OFF(6)|LED_OFF(7)|LED_OFF(8);
    }

    if ((pre_val & (1<<2)) != (usb_buf[1] & (1<<2)))
    {
        /* 中键发生了变化 */
        if (usb_buf[1] & (1<<2))
            *(led_dev.gpiodata) &= LED_ON(5) & LED_ON(6) & LED_ON(7)& LED_ON(8);
        else
            *(led_dev.gpiodata) |= LED_OFF(5)|LED_OFF(6)|LED_OFF(7)|LED_OFF(8);
    }

    pre_val = usb_buf[1];

    /* 重新提交urb */
    usb_submit_urb(uk_urb, GFP_KERNEL);

}

static int usbmouse_as_key_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    struct usb_device *dev = interface_to_usbdev(intf);
    struct usb_host_interface *interface;
    struct usb_endpoint_descriptor *endpoint;
    int pipe;

    interface = intf->cur_altsetting;
    endpoint = &interface->endpoint[0].desc;

	/* a. 分配一个input_dev */
	uk_dev = input_allocate_device();

	/* b. 设置 */
	/* b.1 能产生哪类事件 */
	set_bit(EV_KEY, uk_dev->evbit);
	set_bit(EV_REP, uk_dev->evbit);

	/* c. 注册 */
	//David modified here
	if (namebuf)
	snprintf(namebuf, sizeof(namebuf),
		 "Input David's USB device Vid=0x%04x:Pid=0x%04x",
		 le16_to_cpu(dev->descriptor.idVendor),
		 le16_to_cpu(dev->descriptor.idProduct));
	
	uk_dev->name= namebuf;
	input_register_device(uk_dev);

    /* 数据传输3要素: 源,目的,长度 */
    /* 源: USB设备的某个端点 */
    pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);

    /* 长度: */
    len = endpoint->wMaxPacketSize;

    /* 目的: */
    usb_buf = usb_buffer_alloc(dev, len, GFP_ATOMIC, &usb_buf_phys);

    /* 使用"3要素" */
    /* 分配usb request block */
    uk_urb = usb_alloc_urb(0, GFP_KERNEL);
    /* 使用"3要素设置urb" */
    usb_fill_int_urb(uk_urb, dev, pipe, usb_buf, len, usbmouse_as_key_irq, NULL, endpoint->bInterval);
    uk_urb->transfer_dma = usb_buf_phys;
    uk_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

    /* 使用URB */
    usb_submit_urb(uk_urb, GFP_KERNEL);


    /*硬件设置*/
    led_dev.gpiocon  = (volatile unsigned long*)ioremap(CON_REG, 4);
    led_dev.gpiodata = (volatile unsigned long*)ioremap(DATA_REG,4);

    /* 配置GPB5,6,7,8为输出 */
    *(led_dev.gpiocon) &= ~(LED_MSK(5) |LED_MSK(6) | LED_MSK(7) | LED_MSK(8));
    *(led_dev.gpiocon) |= EN_LED(5)|EN_LED(6)|EN_LED(7)|EN_LED(8);

    return 0;
}

static void usbmouse_as_key_disconnect(struct usb_interface *intf)
{
    struct usb_device *dev = interface_to_usbdev(intf);

    //printk("disconnect usbmouse!\n");
    usb_kill_urb(uk_urb);
    usb_free_urb(uk_urb);

    usb_buffer_free(dev, len, usb_buf, usb_buf_phys);
    input_unregister_device(uk_dev);
    input_free_device(uk_dev);
}

/* 1. 分配/设置usb_driver */
static struct usb_driver usbmouse_as_key_driver = {
    .name       = "usbmouse_as_key_",
    .probe      = usbmouse_as_key_probe,
    .disconnect = usbmouse_as_key_disconnect,
    .id_table   = usbmouse_as_key_id_table,
};


static int usbmouse_as_key_init(void)
{
    /* 2. 注册 */
    usb_register(&usbmouse_as_key_driver);
    return 0;
}

static void usbmouse_as_key_exit(void)
{
    usb_deregister(&usbmouse_as_key_driver);    
}

module_init(usbmouse_as_key_init);
module_exit(usbmouse_as_key_exit);

MODULE_LICENSE("GPL");

