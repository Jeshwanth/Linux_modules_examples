#include<linux/init.h>
#include<linux/module.h>
#include<linux/gpio.h>
#include<linux/io.h>
#include<linux/kernel.h>
#include<plat/gpio.h>
#include<plat/am33xx.h>
#include<linux/irq.h>
#include<linux/interrupt.h>
#include"mux33xx.h" //http://e2e.ti.com/cfs-file.ashx/__key/communityserver-discussions-components-files/791/1641.mux33xx.h

#define DEVICE "GPIO_IN"
#define GPIO1_6 38
#define RECIEVE_ENABLE_MUX 0x27

// control_module_start is the mapped address
static __iomem unsigned char *control_module_start;

// Interrupt handler.
static irqreturn_t gpio_irq_handler ( int irq, void *dev_id)
{
	// Here we can do whatever we want when interrupt occurs: Eg. Send signal to userspace.
	return IRQ_HANDLED;
}

// Set receive enable bit: This should set when you want to read a pin.
int muxRecieveEnable()
{
	unsigned int value = 0;
	
	printk(KERN_INFO " Setting recieve enable bit in mux \n");


	// Get access to the control module.
	request_mem_region (AM33XX_CONTROL_PADCONF_MUX_PBASE, 0x1000, DEVICE);

	// Map io memory.
	control_module_start = ioremap (AM33XX_CONTROL_PADCONF_MUX_PBASE, 0x1000);

	if ( control_module_start == NULL )
	{
		printk (KERN_INFO "Cannot map control module module memory \n");
		return -EIO;
	}
	else
	{
		// Write 0x27 to gpmc_ad6 register.
		__raw_writel ( RECIEVE_ENABLE_MUX, control_module_start + AM33XX_CONTROL_PADCONF_GPMC_AD6_OFFSET );

		// read the gpmc_ad6 value. 
		value = __raw_readl ( control_module_start + AM33XX_CONTROL_PADCONF_GPMC_AD6_OFFSET );
	}

	// Check whether 0x27 is set to gpmc_ad6.
	if ( RECIEVE_ENABLE_MUX != value )
	{
		printk (KERN_INFO "Couldn't set value, expected 0x27 but got 0x%x - Exiting \n",value);
		iounmap ( control_module_start );
		release_mem_region ( AM33XX_CONTROL_PADCONF_MUX_PBASE, 0x1000 );
		return -EIO;
	}
return 0;
}

// Module init: Check the GPIO pin, Make it as input, install the irq.
static int gpio_init(void)
{
	int status = 0;
	bool valid = false;
	int value = 0, int ret = 0;

	// Set mux recieve enable bit.
	muxRecieveEnable();

	// Check GPIO valid or not.
	valid = gpio_is_valid ( GPIO1_6 );

	if ( valid == 0 )
	{
		printk (KERN_ALERT "Valid GPIO pin!!! \n");
	}
	else
	{
		printk (KERN_ALERT "InValid GPIO pin!!! \n");
	}

	// Request a GPIO.
	status = gpio_request ( GPIO1_6, "GPIO1_6" );

	if ( status < 0 )
	{
		printk (KERN_ALERT "GPIO request Failed!!! \n");
	}
	else
	{
		printk (KERN_ALERT "GPIO request successful!!!  \n");
	}

	// Make GPIO pin as input pin.
	gpio_direction_input ( GPIO1_6 );

	// Read the value in Pin 38
	value = gpio_get_value ( GPIO1_6 );

	printk (KERN_ALERT "GPIO value is %d \n", value);

	// Get the IRQ value
	ret = gpio_to_irq ( GPIO1_6 );

	// Install interrupt Handler. Interrupt for BOTH edge.
	ret = request_irq ( ret, gpio_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, DEVICE, NULL );

	if ( ret != 0 )
	{
		printk (KERN_INFO "Error: request_irq returned %d\n", ret);
		return -EIO;
	}

	return 0;
}

// Module exit function.
static void gpio_exit ( void )
{
	printk (KERN_ALERT "Exiting from GPIO Interrupt module!!! \n");
	
	// free the GPIO pin.
	gpio_free ( GPIO1_6 );

	// Free the interrupt request.
	free_irq ( gpio_to_irq ( GPIO1_6 ), NULL );
}


module_init ( gpio_init );
module_exit ( gpio_exit );

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jeshwanth");
MODULE_DESCRIPTION("GPIO Interrupt Kernel Module");
