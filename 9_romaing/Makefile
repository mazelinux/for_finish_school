obj-m := hello.o driver_simple.o
KERNELBUILD :=/lib/modules/`uname -r`/build
default:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	rm -rf *.o *.ko *.mod.c .*.cmd .tmp_versions
