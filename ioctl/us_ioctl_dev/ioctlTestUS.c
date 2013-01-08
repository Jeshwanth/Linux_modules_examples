/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/* 
 * device specifics, such as ioctl numbers and the
 * major device file. 
 */
#include "ioctlTestUS.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */

ioctl_IO(int file_desc)
{
	int FromKernel;

	printf("Received value from Kernel:\n");

	FromKernel = ioctl(file_desc, IOCTL_IO, 20);
	printf("%d\n",FromKernel);
}

/* 
 * Main - Call the ioctl functions 
 */
main()
{

	int file_desc;

	file_desc = open("/dev/ioctlTest", 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	ioctl_IO(file_desc);

	close(file_desc);
}
