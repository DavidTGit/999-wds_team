#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <mach/hardware.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include "mylcd.h"

#if defined(LCD_FB_T240320)
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 170000

#define LCD_RIGHT_MARGIN 25
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 4

#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1
#endif

#define	GPG4	(3<<8)

#define MACH_MINI2440_GPBCON 0x56000010
#define MACH_MINI2440_GPCCON 0x56000020
#define MACH_MINI2440_GPDCON 0x56000030
#define MACH_MINI2440_GPGCON 0x56000060

static struct resource mylcd_resource[] = {
        [0] = {
                .start = MACH_MINI2440_GPBCON,
                .end   = MACH_MINI2440_GPBCON + 8 -1,
                .flags = IORESOURCE_MEM
        },
        [1] = {
                .start = MACH_MINI2440_GPCCON,
                .end   = MACH_MINI2440_GPCCON + 4 - 1,
                .flags = IORESOURCE_MEM
        },
        [2] = {
                .start = MACH_MINI2440_GPDCON,
                .end   = MACH_MINI2440_GPDCON + 4 - 1,
                .flags = IORESOURCE_MEM
        },
        [3] = {
                .start = MACH_MINI2440_GPGCON,
                .end   = MACH_MINI2440_GPGCON + 4 - 1,
                .flags = IORESOURCE_MEM
        }
};

static struct s3c2410fb_display mylcd_pdata __initdata = {
	.width		= LCD_WIDTH,
	.height		= LCD_HEIGHT,
	.pixclock	= LCD_PIXCLOCK,
	.xres		= LCD_WIDTH,
	.yres		= LCD_HEIGHT,
	.bpp		= 16,
	.left_margin	= LCD_LEFT_MARGIN + 1,
	.right_margin	= LCD_RIGHT_MARGIN + 1,
	.hsync_len	= LCD_HSYNC_LEN + 1,
	.upper_margin	= LCD_UPPER_MARGIN + 1,
	.lower_margin	= LCD_LOWER_MARGIN + 1,
	.vsync_len	= LCD_VSYNC_LEN + 1,
};

static struct platform_device mylcd_device = {
        .name           = "mylcd",
        .id             = -1,
        .num_resources  = ARRAY_SIZE(mylcd_resource),
        .resource       = mylcd_resource,
        .dev            = {
                .platform_data  = &mylcd_pdata,
        },
};

static int mylcd_dev_init(void)
{
	platform_device_register(&mylcd_device);
	return 0;
}

static void mylcd_dev_exit(void)
{
	platform_device_unregister(&mylcd_device);
}

module_init(mylcd_dev_init);
module_exit(mylcd_dev_exit);

MODULE_LICENSE("GPL");


