# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard搭建(全部从官方源下载软件包，不做拦截规则等配置，默认开启谷歌原版BBR)

### 国内服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && curl https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/hosts > /etc/hosts && wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

### 海外服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

第一次点击安装后会自动升级系统内核并触发重启，重启后输入

`./WireGuard`

并选1继续安装

### 常见应用:

1.游戏加速器

2.游戏组建局域网联机（MonsterHunter MineCraft GTA5）

### 已知问题:

##### 1.中国移动4G网络会出现断流(当服务器搭建在国外)

解决方案:UDP阻断机制，可以配合UDP2raw规避或者搭建国内跳转

##### 2.DNS有关

解决方案:默认使用服务器自动获取的DNS，可在/etc/wireguard/dns.info中修改

### 客户端下载:

[Windows x64](https://download.wireguard.com/windows-client/wireguard-amd64-0.3.5.msi) [Windows x86](https://download.wireguard.com/windows-client/wireguard-x86-0.3.5.msi) [Android](https://f-droid.org/repo/com.wireguard.android_485.apk) [iOS](https://itunes.apple.com/us/app/wireguard/id1441195209?ls=1&mt=8)

### 使用方法:

Android客户端支持二维码扫描，请直接扫描二维码导入配置，电脑客户端可以复制二维码上方文字配置
