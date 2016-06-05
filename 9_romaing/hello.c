#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
MODULE_LICENSE("Dual BSD/GPL");

//-----------------export----------------//
extern char *str;
//-----------------export----------------//

char* print_process(void) {
	int j;
	char *tmp = kzalloc((4000), GFP_KERNEL);//count must have a given number
	int pid_no = current->pid;
	struct task_struct * task = current;
		j = sprintf(tmp, "process id = %d command= %s state= %d\n",(int)pid_no,current->comm,(int)current->state);
	for_each_process(task) 
	{
		if(task->pid > 50) break;
		j += sprintf(tmp+j, "process id = %d command= %s state= %d\n",(int)task->pid,task->comm,(int)task->state);
	}
	return tmp;
}

static int hello_init(void) {
	//print_process;
	printk(KERN_ALERT "Insmod hello.ko\n");
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Rmmod hello.ko\n");
}

module_init(hello_init);
module_exit(hello_exit);
EXPORT_SYMBOL(print_process);
EXPORT_SYMBOL(str);
