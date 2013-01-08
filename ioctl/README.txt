1. ks_ioctl_dev --> contains kernel module.
2. us_ioctl_dev --> contains user space program.

compile the ks_ioctl_dev us_ioctl_dev by using "make all".

install the module by giving command -- "sudo insmod ioctlTestKS.ko"

Create a device node in /dev directory using -- "sudo mknod /dev/ioctlTest c 100 0"

Run the program "ioctlTestUS", 

See the passed value from US to KS in "dmesg".

See the output of "ioctlTestUS" for value sent from KS.

