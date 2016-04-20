/*======================================================================
  A globalmem driver as an example of char device drivers  

  The initial developer of the original code is Baohua Song
  <author@linuxdriver.cn>. All Rights Reserved.
  ======================================================================*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <driver_simple.h>

#include "driver_simple.h"

static int driversimple_major = DRIVERSIMPLE_MAJOR;
module_param(driversimple_major, int, S_IRUGO);

struct driversimple_dev *driversimple_devp;                                   
/*open function*/
int driversimple_open(struct inode *inode, struct file *filp)
{
		/*将设备结构体指针赋值给文件私有数据指针*/
		struct driversimple_dev *dev;
		int num = MINOR(inode->i_rdev); 
		if (num >= DRIVERSIMPLE_MAXNR)
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

/* ioctl function */
static int driversimple_ioctl(struct inode *inodep, struct file *filp, unsigned	int cmd, unsigned long arg)
{
		int err = 0;
		int ret = 0;
		int ioarg = 0;

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

		//struct driversimple_dev *dev = filp->private_data;/*get point*/

		switch (cmd)
		{
				case DRIVERSIMPLE_PRINT_PROCESS:
						memset(dev->mem, 0, GLOBALMEM_SIZE);      
						printk(KERN_INFO "globalmem is set to zero/n");
						break;
				case DRIVERSIMPLE_SYS_CALL:
						memset(dev->mem, 0, GLOBALMEM_SIZE);      
						printk(KERN_INFO "globalmem is set to zero/n");
						break;
				case DRIVERSIMPLE_SYS_IRT:
						memset(dev->mem, 0, GLOBALMEM_SIZE);      
						printk(KERN_INFO "globalmem is set to zero/n");
						break;
				case DRIVERSIMPLE_SYS_ROAMING:
						memset(dev->mem, 0, GLOBALMEM_SIZE);      
						printk(KERN_INFO "globalmem is set to zero/n");
						break;

				default:
						return  - EINVAL;
		}
		return 0;
}

/*read function*/
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size,
				loff_t *ppos)
{
		unsigned long p =  *ppos;
		unsigned int count = size;
		int ret = 0;
		struct globalmem_dev *dev = filp->private_data; /*get point*/

		/*P>globalmem_size,too big*/
		//if (p >= GLOBALMEM_SIZE)
		if (p > GLOBALMEM_SIZE)
				return count ?  - ENXIO: 0;
		if (count > GLOBALMEM_SIZE - p)
				count = GLOBALMEM_SIZE - p;

		/*kernel space->user space*/
		if (copy_to_user(buf, (void*)(dev->mem + p), count))
		{
				ret =  - EFAULT;
		}
		else
		{
				*ppos += count;
				ret = count;

				printk(KERN_INFO "read %d bytes(s) from %d/n", count, p);
		}

		return ret;
}

/*write function*/
static ssize_t globalmem_write(struct file *filp, const char __user *buf,
				size_t size, loff_t *ppos)
{
		unsigned long p =  *ppos;
		unsigned int count = size;
		int ret = 0;
		struct globalmem_dev *dev = filp->private_data; 

		if (p >= GLOBALMEM_SIZE)
				return count ?  - ENXIO: 0;
		if (count > GLOBALMEM_SIZE - p)
				count = GLOBALMEM_SIZE - p;

		if (copy_from_user(dev->mem + p, buf, count))
				ret =  - EFAULT;
		else
		{
				*ppos += count;
				ret = count;

				printk(KERN_INFO "written %d bytes(s) from %d/n", count, p);
		}

		return ret;
}

/* seek文件定位函数 */
static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
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
						if ((unsigned int)offset > GLOBALMEM_SIZE)
						{
								ret =  - EINVAL;
								break;
						}
						filp->f_pos = (unsigned int)offset;
						ret = filp->f_pos;
						break;
				case 1:   /*相对文件当前位置偏移*/
						if ((filp->f_pos + offset) > GLOBALMEM_SIZE)
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

static const struct file_operations globalmem_fops =
{
		.owner = THIS_MODULE,
		.llseek = globalmem_llseek,
		.read = globalmem_read,
		.write = globalmem_write,
		//.compat_ioctl = globalmem_ioctl,
		.unlocked_ioctl = globalmem_ioctl,
		.open = globalmem_open,
		.release = globalmem_release,
};

/*init cdev*/
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
		int err, devno = MKDEV(globalmem_major, index);

		cdev_init(&dev->cdev, &globalmem_fops);
		dev->cdev.owner = THIS_MODULE;
		dev->cdev.ops = &globalmem_fops;
		err = cdev_add(&dev->cdev, devno, 1);
		if (err)
				printk(KERN_NOTICE "Error %d adding LED%d", err, index);
}

/*设备驱动模块加载函数*/
static int __init  globalmem_init(void)
{
		int result;
		dev_t devno = MKDEV(globalmem_major, 0);

		/* 申请设备号*/
		if (globalmem_major)
				result = register_chrdev_region(devno, 1, "globalmem");
		else  /* 动态申请设备号 */
		{
				result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
				globalmem_major = MAJOR(devno);
		}  
		if (result < 0)
				return result;

		/* 动态申请设备结构体的内存*/
		globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
		if (!globalmem_devp)    /*申请失败*/
		{
				result =  - ENOMEM;
				goto fail_malloc;
		}
		//  memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

		globalmem_setup_cdev(globalmem_devp, 0);
		return 0;

fail_malloc: 
		unregister_chrdev_region(devno, 1);
		return result;
}

/*模块卸载函数*/
static void __exit globalmem_exit(void)
{
		cdev_del(&globalmem_devp->cdev);   /*注销cdev*/
		kfree(globalmem_devp);     /*释放设备结构体内存*/
		unregister_chrdev_region(MKDEV(globalmem_major, 0), 1); /*释放设备号*/
}

MODULE_AUTHOR("Song Baohua");
MODULE_LICENSE("Dual BSD/GPL");

//module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);

