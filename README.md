# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard纯手工搭建教程，不依赖第三方脚本(本项目中代码完全开源，可自行查看分析功能！)

### 直接运行(仅支持CentOS7)：
```
sudo yum install -y gcc wget && echo "151.101.108.133 raw.githubusercontent.com" >> /etc/hosts && echo "52.78.231.108 github.com" >> /etc/hosts && echo "3.81.0.123 copr.fedorainfracloud.org" >> /etc/hosts && wget https://github.com/HXHGTS/WireGuardServer/raw/master/WireGuard.c && sudo chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && sudo ./WireGuard
```
第一次点击安装后会自动升级系统内核并触发重启，重启后输入
```
sudo ./WireGuard
```
并选1继续安装

### 已知问题：

1.客户端（非服务器）监听端口过高（大概50000以上）会导致连接失败，且多出现于移动4G网络，宽带上暂未发现这一问题
```
（解决方案:在用户配置中增加一行 ListenPort = xxxxx）xxxxx最好不超过20000
```
2.若使用Tunsafe客户端会因为脚本中设置了双DNS导致无法加载配置文件
```
（解决方案:在用户配置中 DNS = 8.8.8.8,8.8.4.4）中删除一个DNS，记得把逗号也删除，即可正常使用（下方客户端为官方版，不会出现此问题）
```
### 客户端下载:

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf) [iOS](https://itunes.apple.com/us/app/wireguard/id1441195209?ls=1&mt=8)

### 使用方法:

Android客户端支持二维码扫描，请直接扫描二维码导入配置，电脑客户端可以复制二维码上方文字配置
