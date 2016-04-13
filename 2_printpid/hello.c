#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void) {
    printk(KERN_ALERT "current pid=%i  command=%s ",current->pid,current->comm);
    printk(KERN_ALERT "\n");

    int pid_no=current->pid;
    struct task_struct * task=current;
    
   for_each_process(task) 
   {
      if(task->pid==pid_no) break;
      //mm=task->active_mm;
      printk(KERN_ALERT "process id = %d tpid= %d ",(int)task->pid,(int)task->comm);
   }
printk(KERN_ALERT "\n");
return 0;
}

static void hello_exit(void) {
printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);


 

