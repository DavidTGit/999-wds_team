KERN_DIR = /media/Big/works/friendarm/4418/working-5.1/kernel/linux-3.4.y

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= usbmouse_as_key.o
