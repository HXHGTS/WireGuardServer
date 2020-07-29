# WireGuardServer

## 一、更新内核(Kernel版本低于5必须执行！)

sudo yum update -y

sudo yum install -y wget

wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh

sudo bash preload.sh

## 二、安装WireGuard

直接运行简化版本(基于c语言自己编译，源代码可以查看，避免后门)：

sudo yum install -y gcc

wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c

gcc -o WireGuard WireGuard.c

sudo chmod +x WireGuard

sudo ./WireGuard

客户端下载(原版wg)：

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf)


客户端下载(Tunsafe)：

[Windows x64](https://wwa.lanzous.com/iAvZaf3x1he) [Windows x86]() [Android]()

