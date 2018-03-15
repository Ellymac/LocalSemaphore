#!/bin/bash


if [ $# -ge 1 ];then
  ip="192.168.56.100"
else
  ip="192.168.56.16"
fi

echo $ip

pathKernel="~/data/linux-2.6.30-ise/kernel/"
pathSched="~/data/linux-2.6.30-ise/include/linux/"
pathTest="/home/root/"

echo `scp test/*.c root@$ip:$pathTest`
echo `scp test/*.h root@$ip:$pathTest`

echo ̀`scp src/new_semaphore.c root@$ip:$pathKernel`

echo ̀`scp src/sched.h root@$ip:$pathSched`

echo ̀`scp src/syscall_table_32.S root@$ip:~/data/linux-2.6.30-ise/arch/x86/kernel/`

echo ̀`scp src/Makefile root@$ip:$pathKernel`
