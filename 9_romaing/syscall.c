#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

extern char *str;
// 由于sys_call_table符号不再被导出，需要hardcode地址，
// 地址需要在bash下键入下面命令进行查找：
// $ grep sys_call_table /boot/System.map-`uname -r`
unsigned long *sys_call_table = (unsigned long*) 0xffffffff81801400;
int j = 0;
char *tmp;
//asmlinkage int (*original_open)(const char *, int, int);
/*asmlinkage int hijack_open(const char *filename, int flags, int mode) {
// do hijack logic, just print the parameter
printk(KERN_INFO "hijack: open(%s, %d, %d)\n", filename, flags, mode);
return original_open(filename, flags, mode);
}*/

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
//		printk(KERN_INFO "mkdir=%p\n",__NR_mkdir);
	//	sys_call_for_test();
		//original_open = (void*)*(sys_call_table + __NR_open);
//		printk(KERN_INFO "original_mkdir=%p\n",original_mkdir);
		//*(sys_call_table + __NR_open) = (unsigned long)hijack_open;
//		printk(KERN_INFO "hijack_mkdir=%p\n",hijack_mkdir);
		return 0;
}
void cleanup_module(void) {
	//	no_sys_call_for_test();
		printk(KERN_INFO "Rmmod syscall.ko\n");
}

EXPORT_SYMBOL(sys_call_for_test);
EXPORT_SYMBOL(no_sys_call_for_test);
