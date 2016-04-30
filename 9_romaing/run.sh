#!/bin/bash
PROC=create_proc.ko
PRINT=hello.ko
SYSCALL=syscall.ko
IDT=idt.ko
DRIVER=driver_simple.ko
DEVICE=driver_simple

if [[ $EUID -ne 0 ]]; then
  echo "You must be a root user" 2>&1
  exit 1

make

insmod $PROC
insmod $PRINT
insmod $SYSCALL
insmod $IDT
insmod $DRIVER
mknod /dev/$(DEVICE) c 230 0
if [[ $? == 0 ]];then
	echo "register ok"
	exit 1
