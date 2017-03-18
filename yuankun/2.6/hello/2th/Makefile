KERN_DIR = /home/yk/work/kernel/linux-2.6.32.2

all:
	make -C $(KERN_DIR) M=`pwd` CROSS_COMPILE=arm-linux- modules 

clean:
	make -C $(KERN_DIR) M=`pwd` CROSS_COMPILE=arm-linux- modules clean
	rm -rf modules.order

obj-m	+= hello2.o
