# WireGuardServer

## 一、更新内核(Kernel版本低于5必须执行！)

sudo yum update -y

sudo yum install -y wget

wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh

sudo bash preload.sh

## 二、安装WireGuard

直接运行简化版本(基于c语言自己编译，源代码可以查看，避免后门)：

yum install -y gcc

wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c

gcc -o WireGuard WireGuard.c

chmod +x WireGuard

./WireGuard

