/*======================================================================
  A globalmem driver as an example of char device drivers  

  The initial developer of the original code is Baohua Song
  <author@linuxdriver.cn>. All Rights Reserved.
  ======================================================================*/

//-----------------include----------------//
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h> 
#include <linux/fcntl.h>
#include <linux/seq_file.h>

#include <linux/sched.h>

#include "driver_simple.h"
MODULE_AUTHOR("Maze.ma");
MODULE_LICENSE("GPL");
//-----------------include----------------//
//-----------------export----------------//
	extern int print_process(char *str);
	extern char *str;
//-----------------export----------------//

struct driversimple_dev
{
    struct cdev cdev;
    unsigned char mem[DRIVERSIMPLE_SIZE];
};

static int driversimple_major = DRIVERSIMPLE_MAJOR;
module_param(driversimple_major, int, S_IRUGO);

struct driversimple_dev *driversimple_devp;                                   
/*open function*/
int driversimple_open(struct inode *inode, struct file *filp)
{
		/*将设备结构体指针赋值给文件私有数据指针*/
		struct driversimple_dev *dev;
		int num = MINOR(inode->i_rdev); 
		if (num >= DRIVERSIMPLE_NR_DEVS)
				return -ENODEV;
		dev = &driversimple_devp[num];
		filp->private_data = dev;
		return 0;
}

/*release function*/
int driversimple_release(struct inode *inode, struct file *filp)
{
		return 0;
}


/*read function*/
static ssize_t driversimple_read(struct file *filp, char __user *buf, size_t size,
				loff_t *ppos)
{
		unsigned long p =  *ppos;
		unsigned int count = size;
		int ret = 0;
		struct driversimple_dev *dev = filp->private_data; /*get point*/

		/*P>DRIVERSIMPLE,too big*/
		//if (p >= DRIVERSIMPLE_SIZE)
		if (p > DRIVERSIMPLE_SIZE)
				return count ?  - ENXIO: 0;
		if (count > DRIVERSIMPLE_SIZE - p)
				count = DRIVERSIMPLE_SIZE - p;

		/*kernel space->user space*/
		if (copy_to_user(buf, (void*)(dev->mem + p), count))
		{
				ret =  - EFAULT;
		}
		else
		{
				*ppos += count;
				ret = count;

				printk(KERN_INFO "read %d bytes(s) from %d/n", (int)count, (int)p);
		}

		return ret;
}

/*write function*/
static ssize_t driversimple_write(struct file *filp, const char __user *buf,
				size_t size, loff_t *ppos)
{
		unsigned long p =  *ppos;
		unsigned int count = size;
		int ret = 0;
		struct driversimple_dev *dev = filp->private_data; 

		if (p >= DRIVERSIMPLE_SIZE)
				return count ?  - ENXIO: 0;
		if (count > DRIVERSIMPLE_SIZE - p)
				count = DRIVERSIMPLE_SIZE - p;

		if (copy_from_user(dev->mem + p, buf, count))
				ret =  - EFAULT;
		else
		{
				*ppos += count;
				ret = count;

				printk(KERN_INFO "written %d bytes(s) from %d/n", (int)count, (int)p);
		}

		return ret;
}

/* seek文件定位函数 */
static loff_t driversimple_llseek(struct file *filp, loff_t offset, int orig)
{
		loff_t ret = 0;
		switch (orig)
		{
				case 0:   /*相对文件开始位置偏移*/
						if (offset < 0)
						{
								ret =  - EINVAL;
								break;
						}
						if ((unsigned int)offset > DRIVERSIMPLE_SIZE)
						{
								ret =  - EINVAL;
								break;
						}
						filp->f_pos = (unsigned int)offset;
						ret = filp->f_pos;
						break;
				case 1:   /*相对文件当前位置偏移*/
						if ((filp->f_pos + offset) > DRIVERSIMPLE_SIZE)
						{
								ret =  - EINVAL;
								break;
						}
						if ((filp->f_pos + offset) < 0)
						{
								ret =  - EINVAL;
								break;
						}
						filp->f_pos += offset;
						ret = filp->f_pos;
						break;
				default:
						ret =  - EINVAL;
						break;
		}
		return ret;
}

/* ioctl function */
static long driversimple_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
		int err = 0;
		struct driversimple_dev *dev;
		if (_IOC_TYPE(cmd) != DRIVERSIMPLE_MAGIC) 
				return -EINVAL;
		if (_IOC_NR(cmd) > DRIVERSIMPLE_MAXNR) 
				return -EINVAL;

		if (_IOC_DIR(cmd) & _IOC_READ)
				err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
		else if (_IOC_DIR(cmd) & _IOC_WRITE)
				err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
		if (err) 
				return -EFAULT;

		dev = filp->private_data;/*get point*/

		switch (cmd)
		{
				case DRIVERSIMPLE_PRINT_PROCESS:
						printk(KERN_INFO "Choose function DRIVERSIMPLE_PRINT_PROCESS:\n");
						print_process(str);
						break;
				case DRIVERSIMPLE_SYS_CALL:
						printk(KERN_INFO "Choose function DRIVERSIMPLE_SYS_CALL:\n");
						break;
				case DRIVERSIMPLE_SYS_IRT:
						printk(KERN_INFO "Choose function DRIVERSIMPLE_SYS_IRT:\n");
						break;
				case DRIVERSIMPLE_SYS_ROAMING:
						printk(KERN_INFO "Choose function DRIVERSIMPLE_SYS_ROAMING:\n");
						break;
				default:
						return  - EINVAL;
		}
		return 0;
}

static const struct file_operations driversimple_fops =
{
		.owner = THIS_MODULE,
		.llseek = driversimple_llseek,
		.read = driversimple_read,
		.write = driversimple_write,
		//.compat_ioctl = driversimple_ioctl,
		.unlocked_ioctl = driversimple_ioctl,
		.open = driversimple_open,
		.release = driversimple_release,
};
/*init cdev*/
static void driversimple_setup_cdev(struct driversimple_dev *dev, int index)
{
		int err, devno = MKDEV(driversimple_major, index);

		cdev_init(&dev->cdev, &driversimple_fops);
		dev->cdev.owner = THIS_MODULE;
		dev->cdev.ops = &driversimple_fops;
		err = cdev_add(&dev->cdev, devno, DRIVERSIMPLE_NR_DEVS);
		if (err)
				printk(KERN_NOTICE "Error %d adding LED%d", err, index);
}


/*设备驱动模块加载函数*/
static int __init  driversimple_init(void)
{
		int result;
		dev_t devno = MKDEV(driversimple_major, 0);
		printk(KERN_INFO "init driversimple.ko\n");

		/* 申请设备号*/
		if (driversimple_major)
				result = register_chrdev_region(devno, 1, "driversimple");
		else  /* 动态申请设备号 */
		{
				result = alloc_chrdev_region(&devno, 0, 1, "driversimple");
				driversimple_major = MAJOR(devno);
		}  
		if (result < 0)
				return result;

		/* 动态申请设备结构体的内存*/
		driversimple_devp = kmalloc(sizeof(struct driversimple_dev), GFP_KERNEL);
		if (!driversimple_devp)    /*申请失败*/
		{
				result =  - ENOMEM;
				goto fail_malloc;
		}
		memset(driversimple_devp, 0, sizeof(struct driversimple_dev));

		driversimple_setup_cdev(driversimple_devp, 0);
		return 0;

fail_malloc: 
		unregister_chrdev_region(devno, 1);
		return result;

}

/*模块卸载函数*/
static void __exit driversimple_exit(void)
{
		cdev_del(&driversimple_devp->cdev);   /*注销cdev*/
		kfree(driversimple_devp);     /*释放设备结构体内存*/
		unregister_chrdev_region(MKDEV(driversimple_major, 0), 1); /*释放设备号*/
		printk(KERN_INFO "exit success\n");
}


//module_param(globalmem_major, int, S_IRUGO);

module_init(driversimple_init);
module_exit(driversimple_exit);

