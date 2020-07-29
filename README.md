# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard纯手工搭建教程，不依赖第三方脚本

直接运行简化版本：

sudo yum install -y wget gcc

wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c

gcc -o WireGuard WireGuard.c

sudo chmod +x WireGuard

sudo ./WireGuard
