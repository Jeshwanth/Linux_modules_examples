/*
 *  chardev.h - the header file with the ioctl definitions.
 */

#ifndef IOCTL_TEST_H
#define IOCTL_TEST_H

#include <linux/ioctl.h>

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctl need to know
 * it. 
 */
#define MAJOR_NUM 100


/* 
 * IOCTL ioput and output ( read and write )
 */
#define IOCTL_IO _IOWR(MAJOR_NUM, 2, int)

/* 
 * The IOCTL is used for both input and output. It 
 * receives from the user a number, n, and returns 
 * the number x.
 */

/* 
 * The name of the device file 
 */
#define DEVICE_FILE_NAME "ioctlTest"

#endif
