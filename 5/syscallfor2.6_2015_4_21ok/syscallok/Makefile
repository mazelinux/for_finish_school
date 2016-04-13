obj-m :=getsyscall.o
EXTRA_CFLAGS := -Dsymname=sys_call_table
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(RM) -rf .*.cmd *.mod.c *.o *.ko .tmp*
