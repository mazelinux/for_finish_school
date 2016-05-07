#!/bin/bash
PROC=create_proc
PRINT=hello
SYSCALL=syscall
IDT=idt
DRIVER=driver_simple

if [[ $EUID -ne 0 ]]; then
  echo "You must be a root user" 2>&1
  exit 1
fi


rmmod $PROC
rmmod $PRINT
rmmod $SYSCALL
rmmod $IDT
rmmod $DRIVER
rm /dev/$DRIVER
make clean

if [[ $? == 0 ]];then
    echo "register ok"
fi


