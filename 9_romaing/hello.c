#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("Dual BSD/GPL");

int print_process(void) {
	int pid_no=current->pid;
	struct task_struct * task=current;
	printk(KERN_ALERT "current pid=%d  command=%s  state=%d",(int)current->pid,current->comm,(int)current->state);
	//state -1 = unrunnable,0 = runnable, >0 = stopped
	printk(KERN_ALERT "\n");
	for_each_process(task) 
	{
		if(task->pid==pid_no) break;
		//mm=task->active_mm;
		printk(KERN_ALERT "process id = %d command= %s state= %d",(int)task->pid,task->comm,(int)task->state);
		printk(KERN_ALERT "\n");
	}
	printk(KERN_ALERT "\n");
	return 0;
}

static int hello_init(void) {
	int pid_no=current->pid;
	struct task_struct * task=current;
	printk(KERN_ALERT "current pid=%d  command=%s  state=%d",(int)current->pid,current->comm,(int)current->state);
	//state -1 = unrunnable,0 = runnable, >0 = stopped
	printk(KERN_ALERT "\n");
	for_each_process(task) 
	{
		if(task->pid==pid_no) break;
		//mm=task->active_mm;
		printk(KERN_ALERT "process id = %d command= %s state= %d",(int)task->pid,task->comm,(int)task->state);
		printk(KERN_ALERT "\n");
	}
	printk(KERN_ALERT "\n");
	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
EXPORT_SYMBOL(print_process);



