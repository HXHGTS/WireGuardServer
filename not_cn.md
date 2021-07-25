### 海外服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

### 海外服务器直接运行(支持v4/v6双栈网络)(仅支持CentOS7):

`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/WireGuard_ipv6.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

### 海外服务器直接运行(支持FULLCONE)(仅支持CentOS7):

`yum install -y gcc wget && wget https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/WireGuard_FullCone.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

第一次点击安装后会自动升级系统内核并触发重启，重启后输入

`./WireGuard`

并选1继续安装

[返回](./README.md)
