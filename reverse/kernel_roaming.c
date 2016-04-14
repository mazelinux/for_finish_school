#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAZE <maze.ma1251228@gmail.com/maze_linux@outlook.com/406732060@qq.com>");
MODULE_DESCRIPTION("Just for fun");
static unsigned long buffer_size = 8192;
module_param( buffer_size, ulong, (S_IRUSR | S_IRGRP | S_IROTH));
MODULE_PARM_DESC(buffer_size,"Internal buffer size");

static int __init reverse_init(void)
{
	if (!buffer_size)
		return -1;
	printk(KERN_INFO"reverse device has been registered\n");
	printk(KERN_INFO"buffer size is %lu bytes\n",buffer_size);
	return 0;
}

static void __exit reverse_exit(void)
{
	printk(KERN_INFO"reverse device has been unregistered\n");
}

module_init(reverse_init);
module_exit(reverse_exit);
