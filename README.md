# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard纯手工搭建教程，不依赖第三方脚本(本项目中代码完全开源，可自行查看分析功能！)

直接运行(仅支持CentOS7)：
```
sudo yum install -y gcc wget && echo "151.101.108.133 raw.githubusercontent.com" >> /etc/hosts && echo "52.78.231.108 github.com" >> /etc/hosts && echo "3.81.0.123 copr.fedorainfracloud.org" >> /etc/hosts && wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c && sudo chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && sudo ./WireGuard
```
第一次点击安装后会自动升级系统内核并触发重启，重启后输入
```
sudo ./WireGuard
```
并选1继续安装

已知问题：

1.客户端（非服务器）监听端口过高（大概50000以上）会导致连接失败，且多出现于移动4G网络，所以脚本新增本地监听端口，以提高4G连接成功率，可以酌情修改（对应用户配置中 ListenPort = xxxxx）

客户端下载:

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf)

