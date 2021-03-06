#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

//-----------------export----------------//
extern char *str;
//-----------------export----------------//
// 由于sys_call_table符号不再被导出，需要hardcode地址，
// 地址需要在bash下键入下面命令进行查找：
// $ grep sys_call_table /boot/System.map-`uname -r`
unsigned long *sys_call_table = (unsigned long*) 0xffffffff81801400;
int j = 0;
char *tmp;

asmlinkage int (*original_mkdir)(const char *);
asmlinkage int hijack_mkdir(const char *filename) {
		// do hijack logic, just print the parameter
		j += sprintf(tmp+j, "hijack: mkdir filename=(%s)\n", filename);
		str = tmp;
		return original_mkdir(filename);
}

int make_ro(unsigned long address) {
		unsigned int level;
		pte_t *pte = lookup_address(address, &level);
		pte->pte = pte->pte &~ _PAGE_RW;
		return 0;
}
int make_rw(unsigned long address) {
		unsigned int level;
		pte_t *pte = lookup_address(address, &level);
		if(pte->pte &~ _PAGE_RW) {
				pte->pte |= _PAGE_RW;
		}
		return 0;
}

void sys_call_for_test(void){
		make_rw((unsigned long)sys_call_table);
		tmp = kzalloc((100), GFP_KERNEL);
		original_mkdir = (void*)*(sys_call_table + __NR_mkdir);
		*(sys_call_table + __NR_mkdir) = (unsigned long)hijack_mkdir;
		make_ro((unsigned long)sys_call_table);
}

void no_sys_call_for_test(void){
		make_rw((unsigned long)sys_call_table);
		*(sys_call_table + __NR_mkdir) = (unsigned long)original_mkdir;
		make_ro((unsigned long)sys_call_table);
}

int init_module(void) {
		printk(KERN_INFO "Insmod syscall.ko\n");
	//	sys_call_for_test();
		return 0;
}
void cleanup_module(void) {
	//	no_sys_call_for_test();
		printk(KERN_INFO "Rmmod syscall.ko\n");
}

EXPORT_SYMBOL(sys_call_for_test);
EXPORT_SYMBOL(no_sys_call_for_test);
