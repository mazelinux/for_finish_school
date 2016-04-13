#ifndef __SYSCALL_INCLUDE__
# define __SYSCALL_INCLUDE__
#endif
#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif 
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xinhe <xinhe007@163.com>");
MODULE_DESCRIPTION("export the sys_call_table");
#if !defined(symname)
    #error symname not defined
#endif
#define CALLOFF 100
unsigned symname; /* #define */
struct {
   unsigned short limit;
   unsigned int base;
} __attribute__ ((packed)) idtr;

struct {
   unsigned short off1;
   unsigned short sel;
   unsigned char none,
   flags;
   unsigned short off2;
} __attribute__ ((packed)) * idt;

void set_symbol_addr(unsigned old_value, unsigned new_value)
{
struct module *mod;
struct kernel_symbol *s;
int i;
for (mod = THIS_MODULE, s = mod->syms, i = 0; i < mod->num_syms; ++i, ++s)
if (s->value == old_value)
{
s->value = new_value;
return;
}
/*遍历本模块的符号表，把本模块的一个也叫sys_call_table的符号的地址
设置为系统真正的sys_call_table的实际地址。 */
}

char * findoffset(char *start)
{
char *p;
for (p = start; p < start + CALLOFF; p++)
if (*(p + 0) == '\xff' && *(p + 1) == '\x14' && *(p + 2) == '\x85')
return p;
/*查找ini 80处理函数对sys_call_table的引用*/
return NULL;
}

static int __init init(void)
{
  unsigned sys_call_off;
  unsigned sct;
  char *p;
  asm("sidt %0":"=m"(idtr));
  idt = (void *) (idtr.base + 8 * 0x80);
  sys_call_off = (idt->off2 << 16) | idt->off1;
  /*查找int 80的入口地址*/
  if ((p = findoffset((char *) sys_call_off)))
  {
    sct = *(unsigned *) (p + 3);
    set_symbol_addr((unsigned) &symname, sct);
  }
  //EXPORT_SYMBOL(sys_call_table);
  return 0;
}

static void __exit fini(void)
{
}
module_init(init);
module_exit(fini);
