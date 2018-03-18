#!/bin/bash

pathKernel="~/data/linux-2.6.30-ise/kernel/"
pathSched="~/data/linux-2.6.30-ise/include/linux/"
pathTest="/home/root/"

ip="192.168.56.16"

echo ̀`scp src/Makefile root@$ip:$pathKernel`

if [ $# -ge 1 ];then
  if [ $1 == 0 ];then
    echo File system
    echo ̀`scp src/sched.h root@$ip:$pathSched`
    echo ̀`scp src/syscalls.h root@$ip:$pathSched`
    echo ̀`scp src/syscall_table_32.S root@$ip:~/data/linux-2.6.30-ise/arch/x86/kernel/`
    echo ̀`scp src/unistd_32.h root@$ip:~/data/linux-2.6.30-ise/arch/x86/include/asm`
  fi
  if [ $1 == 1 ];then
    echo New_semaphore
    echo ̀`scp src/new_semaphore.c root@$ip:$pathKernel`
    echo ̀`scp src/mytest.c root@$ip:$pathKernel`
  fi
  if [ $1 == 2 ];then
    echo Test
    echo `scp test/*.c root@$ip:$pathTest`
    echo `scp test/*.h root@$ip:$pathTest`
  fi
  if [ $1 == 3 ];then
    echo Exit et Fork
    echo ̀`scp src/fork.c root@$ip:$pathKernel`
    echo ̀`scp src/exit.c root@$ip:$pathKernel`
  fi


else
  echo False
fi

echo $ip