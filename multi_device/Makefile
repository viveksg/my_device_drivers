obj-m:=multi_device.obj
ARM_CROSS_COMPILER=arm-none-linux-gnueabihf-
TARGET=arm
HOST_KERNEL_DIR=/lib/modules/$(shell uname -r)/build/
TARGET_KERNEL_DIR=/lib/modules/4.14.108/source

all:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(ARM_CROSS_COMPILER) -C$(TARGET_KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(HOST_KERNEL_DIR) M=$(PWD) clean	\
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(ARM_CROSS_COMPILER) -C$(KERN_DIR) M=$(PWD) clean
help:
	make ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(ARM_CROSS_COMPILER) -C$(KERN_DIR) M=$(PWD) help	
host:
	make -C $(HOST_KERNEL_DIR) M=$(PWD) modules	