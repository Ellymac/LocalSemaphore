#!/bin/bash


if [ $# -ge 1 ];then
  ip="192.168.56.100"
else
  ip="192.168.56.16"
fi

echo $ip

pathKernel="~/data/linux-2.6.30-ise/kernel/"
pathTest="/home/root/"

echo `scp test/*.c root@$ip:$pathTest`
echo ̀`scp src/mytest.c root@$ip:$pathKernel`
