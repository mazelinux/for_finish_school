#ifndef _DRIVERSIMPLE_H_
#define _DRIVERSIMPLE_H_

#include <linux/ioctl.h>

#ifndef DRIVERSIMPLE_MAJOR
#define DRIVERSIMPLE_MAJOR 230   /*预设的mem的主设备号*/
#endif

#ifndef DRIVERSIMPLE_NR_DEVS
#define DRIVERSIMPLE_NR_DEVS 1    /*设备数*/
#endif

#ifndef DRIVERSIMPLE_SIZE
#define DRIVERSIMPLE_SIZE 0x1000 //driver mem 4k
#endif


/* 定义幻数 */
#define DRIVERSIMPLE_MAGIC  'd'

/* 定义命令 */
#define DRIVERSIMPLE_PRINT_PROCESS  _IOW(DRIVERSIMPLE_MAGIC, 1, int)
#define DRIVERSIMPLE_SYS_CALL 		_IOW(DRIVERSIMPLE_MAGIC, 2, int)
#define DRIVERSIMPLE_SYS_IRT 		_IOW(DRIVERSIMPLE_MAGIC, 3, int)
#define DRIVERSIMPLE_SYS_ROAMING  	_IOW(DRIVERSIMPLE_MAGIC, 4, int)
#define DRIVERSIMPLE_MAXNR 4

struct driversimple_dev
{
	struct cdev cdev;
	unsigned char mem[DRIVERSIMPLE_SIZE];
};
#endif /* _DRIVERSIMPLE_H_ */
