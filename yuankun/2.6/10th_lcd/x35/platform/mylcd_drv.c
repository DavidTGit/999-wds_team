#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
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
#include <mach/regs-lcd.h>
#include <mach/regs-gpio.h>
#include <mach/fb.h>


#if (DEBUG)
#define dev_dbg(fmt,msg...) printk(msg)
#define dev_err(fmt,msg...) printk(msg)
#endif
static const char driver_name[] = "mylcd_fb";

static volatile unsigned long *gpbcon;
static volatile unsigned long *gpbdat;
static volatile unsigned long *gpccon;
static volatile unsigned long *gpdcon;
static volatile unsigned long *gpgcon;
static volatile struct lcd_regs* lcd_regs;

# for fb ops
static struct fb_ops s3c_lcdfb_ops = {
	.owner		= THIS_MODULE,
	.fb_setcolreg	= s3c_lcdfb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};

static int __devinit mylcd_probe(struct platform_device *pdev)
{
	struct resource *res;				//get lcd info
	struct fb_info *fbinfo;				//for fb fill up
	struct s3c2410fb_display *display;	//for display timing
	
	display = pdev->dev.platform_data;
	if(pdata == NULL) {
		dev_err(&pdev->dev,
			"no platform data for lcd, cannot attach\n");
		return -EINVAL;
	}
	
	fbinfo = framebuffer_alloc(0,&pdev->dev);
	if(!fbinfo)	
		return -ENOMEM;

	strcpy(fbinfo->fix.id, driver_name);

	/* Stop the video */
//	lcdcon1 = readl(info->io + S3C2410_LCDCON1);
//	writel(lcdcon1 & ~S3C2410_LCDCON1_ENVID, info->io + S3C2410_LCDCON1);

	fbinfo->fix.type	    = FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.type_aux	    = 0;
	fbinfo->fix.xpanstep	    = 0;
	fbinfo->fix.ypanstep	    = 0;
	fbinfo->fix.ywrapstep	    = 0;
	fbinfo->fix.accel	    = FB_ACCEL_NONE;
	fbinfo->fix.visual   = FB_VISUAL_TRUECOLOR; /* TFT */
	fbinfo->fix.line_length = display->xres*4;

	fbinfo->var.nonstd	    = 0;
	fbinfo->var.activate	    = FB_ACTIVATE_NOW;
	fbinfo->var.accel_flags     = 0;
	fbinfo->var.vmode	    = FB_VMODE_NONINTERLACED;

	fbinfo->fbops		    = &s3c_lcdfb_ops;
	fbinfo->flags		    = FBINFO_FLAG_DEFAULT;
	fbinfo->pseudo_palette      = &info->pseudo_pal;
	
	fbinfo->var.xres = display->xres;
	fbinfo->var.yres = display->yres;
	fbinfo->var.xres_virtual   = display->xres;
	fbinfo->var.yres_virtual = display->yres;
	fbinfo->var.bits_per_pixel = display->bpp;
//	fb_var_set(&fbinfo->var, fbinfo); later for complete, now fill directly
		/* RGB:565 */
	fbinfo->var.red.offset     = 16;
	fbinfo->var.red.length     = 8;
	
	fbinfo->var.green.offset   = 8;
	fbinfo->var.green.length   = 8;

	fbinfo->var.blue.offset    = 0;
	fbinfo->var.blue.length    = 8;
	
	fbinfo->screen_size = display->xres*display->yres*32/8;
	fbinfo->fix.smem_len = fbinfo->screen_size;
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpbcon = ioremap(res->start, res->end - res->start + 1);
	gpbdat = gpbcon + 1;
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpccon = ioremap(res->start, res->end - res->start + 1);
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpdcon = ioremap(res->start, res->end - res->start + 1);
	
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpgcon = ioremap(res->start, res->end - res->start + 1);
	
}

static struct platform_driver mylcd_driver = {
	.driver = {
		.name	= "mylcd",
	},
	.probe	= mylcd_probe,
	.remove	= mylcd_remove,
};

static void __init mylcd_init(void) {
	printk(KERN_INFO "Register mylcd driver.\n");
	return platform_driver_register(&mylcd_driver);
}

static void __exit mylcd_exit(void) {
	platform_driver_unregister(&mylcd_driver);
}

module_init(mylcd_init);
module_exit(mylcd_exit);

MODULE_LICENSE("GPL");


