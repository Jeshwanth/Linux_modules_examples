#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/init.h>


static int hello_init(void)
{
   printk("Hello Jeshwanth.\n");
	
   return 0;
}


static void hello_exit(void)
{
  printk(KERN_ALERT "Goodbye world 1.\n");
}  

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
