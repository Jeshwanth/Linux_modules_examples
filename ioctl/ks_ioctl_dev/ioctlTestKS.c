#include <linux/kernel.h>
#include <linux/module.h>   /* This is a Module!!! init module and exit module */
#include <linux/init.h>
#include <linux/fs.h>		/* register_chrdev and unregister_chardev Functions */
#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/types.h>	/* For data types */
#include <linux/printk.h>   /* Using printk */
#include "ioctlTestKS.h"

#define SUCCESS 0
#define DEVICE_NAME "ioctl_test"

/*
 * Is the device open right now? Used to prevent
 * Concurrent access into the same device
 */
static int Device_Open = 0;


/* 
 * This is called whenever a process attempts to open the device file 
 */
static int device_open(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_open(%p)\n", file);
#endif

	/* 
	 * We don't want to talk to two processes at the same time 
	 */
	if (Device_Open)
		return -EBUSY;

	Device_Open++;

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	#ifdef DEBUG
		printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
	#endif

	Device_Open--;

	return SUCCESS;
}

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file.
 */
long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	/* 
	 * Switch according to the ioctl called 
	 */
	switch (ioctl_num)
	{
	case IOCTL_IO:
		/* 
		 * This ioctl is both input (ioctl_param) and 
		 * output (the return value of this function) 
		 */
		printk(KERN_ALERT "I am in IOCTL_IO :\n");
		printk(KERN_ALERT "User Sent me value : %lu\n",ioctl_param);

		/* So I am sending a number now */

		return 30;
		break;

	default:
		printk(KERN_ALERT "IOCTL Command not matching");
		break;
	}

	return SUCCESS;
}

/* Module Declarations */

/* 
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions. 
 */
struct file_operations Fops =
{
	.owner          = THIS_MODULE,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,	/* close */
};

/* 
 * Initialize the module - Register the character device 
 */
int init_module()
{
	int ret_val;
	/* 
	 * Register the character device.
	 */
	ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

	/* 
	 * Negative values signify an error 
	 */
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "character device registration ", ret_val);
		return ret_val;
	}


	printk(KERN_INFO "%s The major device number is %d.\n",
	       "Registeration is a success", MAJOR_NUM);
	printk(KERN_INFO "If you want to talk to the device driver,\n");
	printk(KERN_INFO "you'll have to create a device file. \n");
	printk(KERN_INFO "We suggest you use:\n");
	printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
	printk(KERN_INFO "The device file name is important, because\n");
	printk(KERN_INFO "the ioctl program assumes that's the\n");
	printk(KERN_INFO "file you'll use.\n");

	return 0;
}

/* 
 * Cleanup - unregister the appropriate file from /proc 
 */
void cleanup_module()
{

	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}
