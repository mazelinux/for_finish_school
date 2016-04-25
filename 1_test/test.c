#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE(" GPL ");
MODULE_AUTHOR("Maze.ma");

static int test_init(void)
{
	printk(KERN_INFO "You have insert a module\n");
	return 0;
}

static void test_exit(void)
{	
	printk(KERN_INFO "You pull out a module\n");
}

module_init(test_init);
module_exit(test_exit);
