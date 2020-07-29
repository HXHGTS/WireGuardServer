# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard纯手工搭建教程，不依赖第三方脚本(本项目中代码完全开源，可自行查看分析功能！)

直接运行(仅支持CentOS7)：

sudo yum update -y

sudo yum install -y wget gcc

wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh

sudo chmod +x preload.sh

sudo bash preload.sh

这个执行完成会触发重启且不会提示，是正常情况

wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c

gcc -o WireGuard WireGuard.c

sudo chmod +x WireGuard

sudo ./WireGuard

客户端下载(原版wg)：

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf)


客户端下载(Tunsafe)：

[Windows测试版](https://wwa.lanzous.com/iAvZaf3x1he) [Windows正式版](https://wwa.lanzous.com/ioykQf427sb) [TAP](https://wwa.lanzous.com/iGMaCf42d4d) [Android](https://wwa.lanzous.com/iSUxQf3xdgf)

