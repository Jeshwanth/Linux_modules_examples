/*
 *  ioctlTestUS.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef IOCTL_TEST_US_H
#define IOCTL_TEST_US_H

#include <linux/ioctl.h>

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100

/* 
 * Get the n'th byte of the message 
 */
#define IOCTL_IO _IOWR(MAJOR_NUM, 2, int)
/* 
 * The IOCTL is used for both input and output. It 
 * receives from the user a number, n, and returns 
 * number x.
 */

/*
 * The name of the device file
 */
#define DEVICE_FILE_NAME "ioctlTest"


#endif
