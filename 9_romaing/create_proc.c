#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
MODULE_AUTHOR("Maze.ma");
MODULE_LICENSE("GPL");



//-----------------proc----------------//
//-----------------proc----------------//
//-----------------proc----------------//
//-----------------proc----------------//
char *str = NULL;//this size must given

static int driversimple_proc_show(struct seq_file *seq, void *v)
{
		seq_printf(seq, "%s\n", str);
		return 0;
}

	static int my_read(char* buffer,char** buffer_location,off_t off_set,int* buf_length,int* eof,void* data)
{
	
}

static ssize_t driversimple_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
		//struct seq_file *seq = file->private_data;
		//unsigned int *ptr_var = seq->private;
		//*ptr_var = simple_strtoul(buffer, NULL, 10);
		//return count;
		//with error about null point
		char *tmp = kzalloc((count+1), GFP_KERNEL);  
		if(!tmp)  
				return -ENOMEM;  
		if(copy_from_user(tmp, buffer, count))  
		{  
				kfree(tmp);  
				return EFAULT;  
		}  
		kfree(str);  
		str = tmp;  
		return count; 
}

static int driversimple_proc_open(struct inode *inode, struct file *file)
{
		return single_open(file, driversimple_proc_show, PDE_DATA(inode));
}

static const struct file_operations driversimple_proc_fops =
{
		.owner = THIS_MODULE,
		.open = driversimple_proc_open,
		.read =	seq_read,
		.write = driversimple_proc_write,
		.llseek = seq_lseek,
		.release = single_release,
};

static int driversimple_create_proc(void)
{
		struct proc_dir_entry *entry;
		entry = proc_create_data("dirversimple_file", 0, NULL, &driversimple_proc_fops, NULL);//date=1,printk;date=2,syscall...
		//entry = proc_create_entry("dirversimple_file", 0, NULL);//date=1,printk;date=2,syscall...
		if(!entry)
				return EFAULT;
		return 0;
}

static void driversimple_remove_proc(void)
{
		remove_proc_entry("dirversimple_file", NULL);
}

static int __init create_proc(void)
{
	driversimple_create_proc();
	return 0;
}

static void __exit remove_proc(void)
{	
	driversimple_remove_proc();
}

EXPORT_SYMBOL(str);
module_init(create_proc);
module_exit(remove_proc);
	
//-----------------proc----------------//
//-----------------proc----------------//
//-----------------proc----------------//
//-----------------proc----------------//
