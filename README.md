# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard纯手工搭建教程，不依赖第三方脚本(本项目中代码完全开源，可自行查看分析功能！)

直接运行(仅支持CentOS7)：
```
sudo yum install -y gcc

wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c

sudo chmod +x WireGuard.c

gcc -o WireGuard WireGuard.c

sudo ./WireGuard
```
Kernel版本低于5的务必先进行内核升级再安装！

客户端下载:

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf)

