# WireGuardServer

## 警告⚠：此技术仅限用于个人搭建游戏加速器使用！！！若用于其他违法目的，后果自负！！！

WireGuard搭建(全部从官方源下载软件包，不做拦截规则等配置，默认开启谷歌原版BBR)

### 国内服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && echo '3.81.0.123 copr.fedorainfracloud.org' > /etc/hosts && echo "151.101.76.133 raw.githubusercontent.com' >> /etc/hosts && wget https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && ./WireGuard`

### 海外服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && ./WireGuard`

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

[Windows x64](https://wwa.lanzous.com/i9q09f3x2zi) [Windows x86](https://wwa.lanzous.com/ibVYif3x32b) [Android](https://wwa.lanzous.com/izHzDf3x1sf) [iOS](https://itunes.apple.com/us/app/wireguard/id1441195209?ls=1&mt=8)

### 使用方法:

Android客户端支持二维码扫描，请直接扫描二维码导入配置，电脑客户端可以复制二维码上方文字配置
