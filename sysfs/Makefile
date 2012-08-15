obj-m +=enable_attr_sysfs.o

KDIR= /usr/src/`uname -r`

all:

	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* .c* .t*
