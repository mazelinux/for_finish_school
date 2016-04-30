#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "driver_simple.h"

#define URL /dev/driver_simple
int main()
{
	int fd = 0;
	int cmd;
	int arg = 0;
	char BUF[4096];

	fd = open("/dev/driver_simple",O_RDWR);
	if (fd < 0)
	{
		printf("OPEN  DEVICE ERROR\n");
		return -1;
	}
	printf("-----CALL CMD-----\n");
	printf("-----1: DRIVERSIMPLE_PRINT_PROCESS-----\n");
	printf("-----2: DRIVERSIMPLE_SYS_CALL-----\n");
	printf("-----3: DRIVERSIMPLE_SYS_IRT-----\n");
	printf("-----4: DRIVERSIMPLE_SYS_ROAMING-----\n");
	scanf("%d",&cmd);
	switch(cmd)
	{	
		case 1: 
			cmd=DRIVERSIMPLE_PRINT_PROCESS;
			if (ioctl(fd, cmd, &arg) < 0)
        	{
            	printf("Call cmd DRIVERSIMPLE_PRINT_PROCESS fail\n");
            	return -1;
    		}	
			break;
		case 2:
			cmd=DRIVERSIMPLE_SYS_CALL;
			if (ioctl(fd, cmd, &arg) < 0)
        	{
            	printf("Call cmd DRIVERSIMPLE_SYS_CALL fail\n");
            	return -1;
    		}	
			break;
		case 3:
			cmd=DRIVERSIMPLE_SYS_IRT;
			if (ioctl(fd, cmd, &arg) < 0)
        	{
            	printf("Call cmd DRIVERSIMPLE_SYS_IRT fail\n");
            	return -1;
    		}	
			break;
		case 4:
			cmd=DRIVERSIMPLE_SYS_ROAMING;
			if (ioctl(fd, cmd, &arg) < 0)
        	{
            	printf("Call cmd DRIVERSIMPLE_SYS_ROAMING fail\n");
            	return -1;
    		}	
			break;
		default : printf("error\n");
	}
	close(fd);
	return 0;
}
