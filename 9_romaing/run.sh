#!/bin/bash
PROC=create_proc.ko
PRINT=hello.ko
SYSCALL=syscall.ko
IDT=idt.ko
DRIVER=driver_simple.ko

if [[ $EUID -ne 0 ]]; then
  echo "You must be a root user" 2>&1
  exit 1

make

insmod $PROC
insmod $PRINT
insmod $SYSCALL
insmod $IDT
insmod $DRIVER
