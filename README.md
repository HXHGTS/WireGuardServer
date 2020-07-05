# WireGuardServer

## 一、更新内核
yum -y install epel-release curl

sed -i "0,/enabled=0/s//enabled=1/" /etc/yum.repos.d/epel.repo

yum remove -y kernel-devel

rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org

rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm

yum --disablerepo="*" --enablerepo="elrepo-kernel" list available

yum -y --enablerepo=elrepo-kernel install kernel-ml

sed -i "s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/" /etc/default/grub

grub2-mkconfig -o /boot/grub2/grub.cfg

wget https://mirror.tuna.tsinghua.edu.cn/elrepo/kernel/el7/x86_64/RPMS/kernel-ml-devel-5.7.6-1.el7.elrepo.x86_64.rpm

rpm -ivh kernel-ml-devel-5.7.6-1.el7.elrepo.x86_64.rpm

yum -y --enablerepo=elrepo-kernel install kernel-ml-devel

reboot

## 二、安装WireGuard

curl -Lo /etc/yum.repos.d/wireguard.repo https://copr.fedorainfracloud.org/coprs/jdoss/wireguard/repo/epel-7/jdoss-wireguard-epel-7.repo

yum install epel-release -y

yum install wireguard-dkms wireguard-tools -y

echo "net.ipv4.ip_forward = 1" >> /etc/sysctl.conf

sysctl -p

mkdir -p /etc/wireguard && chmod 0777 /etc/wireguard

cd /etc/wireguard

umask 077

wg genkey | tee server_privatekey | wg pubkey > server_publickey

wg genkey | tee client_privatekey | wg pubkey > client_publickey

参见 服务器配置文件

参见 用户配置文件

systemctl enable wg-quick@wg0


### 启动WireGuard
wg-quick up wg0

### 停止WireGuard
wg-quick down wg0

### 查看WireGuard运行状态
wg

### 停止WireGuard
wg-quick down wg0

### 已经成功创建后，启动WireGuard
wg-quick up wg0

## 用户端配置文件模板client.conf（酌情修改）

[Interface]

PrivateKey = gInWKSooaDEDXwai+KC6ole6bw/P5z21Q2pBIsQhxXw=

Address = 10.0.0.2/24

DNS = 67.207.67.2

DNS = 67.207.67.3

MTU = 1492

[Peer]

PublicKey = SBah8iYP2mYv+5PlEAU9ZRwMllsdz6o+Slt2DDXu1yg=

Endpoint = sqbgp.nat.coalcloud.xyz:21735

AllowedIPs = 0.0.0.0/0

PersistentKeepalive = 25 

## 服务器端配置模板wg0.conf（酌情修改）

[Interface]

PrivateKey = UMez+xG0p9x5UUEnVQYieentswN90rRLbtHotaaL+nw=

Address = 10.0.0.1/24

PostUp = iptables -A FORWARD -i wg0 -j ACCEPT; iptables -A FORWARD -o wg0 -j ACCEPT; iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

PostDown = iptables -D FORWARD -i wg0 -j ACCEPT; iptables -D FORWARD -o wg0 -j ACCEPT; iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE

ListenPort = 21735

DNS = 67.207.67.2

DNS = 67.207.67.3

MTU = 1492

[Peer]

PublicKey = SHlCXsbxOLG8WuACzFR6Pm+zfCLeJlWEgcqELzYD4yU=

AllowedIPs = 10.0.0.2/32 

