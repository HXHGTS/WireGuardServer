## 国内服务器

### 安装前运行

```
yum update -y && yum install -y gcc curl wget && curl -sSL https://raw.iqiq.io/HXHGTS/WireGuardServer/master/hosts > /etc/hosts && reboot
```

### 普通安装:

```
wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard
```

### 支持v4/v6双栈网络:

```
wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard_ipv6.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard
```

### 支持FULLCONE:

```
wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard_FullCone.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard
```

第一次点击安装后会自动升级系统内核并触发重启，重启后输入

```
./WireGuard
```

并选1继续安装

[返回](./README.md)
