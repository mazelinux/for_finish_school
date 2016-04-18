#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/highmem.h>
MODULE_LICENSE("GPL");
// 由于sys_call_table符号不再被导出，需要hardcode地址，
// 地址需要在bash下键入下面命令进行查找：
// $ grep sys_call_table /boot/System.map-3.xx.xx-xx-generic
unsigned long *sys_call_table = (unsigned long*) 0xffffffff81801400;
//unsigned long *sys_call_table = (unsigned long*) 0xffffffff81600480;
asmlinkage int (*original_open)(const char *, int, int);
asmlinkage int hijack_open(const char *filename, int flags, int mode) {
  // do hijack logic, just print the parameter
  printk(KERN_INFO "hijack: open(%s, %d, %d)\n", filename, flags, mode);
  return original_open(filename, flags, mode);
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
int init_module(void) {
  make_rw((unsigned long)sys_call_table);
  original_open = (void*)*(sys_call_table + __NR_open);
  *(sys_call_table + __NR_open) = (unsigned long)hijack_open;
  make_ro((unsigned long)sys_call_table);
  return 0;
}
void cleanup_module(void) {
  make_rw((unsigned long)sys_call_table);
  *(sys_call_table + __NR_open) = (unsigned long)original_open;
  make_ro((unsigned long)sys_call_table);
}
