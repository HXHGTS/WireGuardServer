# WireGuardServer

## 一、更新内核
yum -y install epel-release curl vim wget

sed -i "0,/enabled=0/s//enabled=1/" /etc/yum.repos.d/epel.repo

yum remove -y kernel-devel

rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org

rpm -Uvh https://github.com/HXHGTS/WireGuardServer/raw/master/elrepo-release-7.0-5.el7.elrepo.noarch.rpm

yum --disablerepo="*" --enablerepo="elrepo-kernel" list available

yum -y --enablerepo=elrepo-kernel install kernel-ml

sed -i "s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/" /etc/default/grub

grub2-mkconfig -o /boot/grub2/grub.cfg

wget https://github.com/HXHGTS/WireGuardServer/raw/master/kernel-ml-devel-5.7.8-1.el7.elrepo.x86_64.rpm

rpm -ivh kernel-ml-devel-5.7.8-1.el7.elrepo.x86_64.rpm

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

wg genkey | tee client1_privatekey | wg pubkey > client1_publickey

## 服务器端配置模板/etc/wireguard/wg0.conf（酌情修改）

[Interface]

PrivateKey = 0BH2tb+3uMzdsMW0ODzZasndJPXLX/0LGZByalTHom4= 

Address = 10.0.0.1/16 

PostUp   = iptables -A FORWARD -i wg0 -j ACCEPT; iptables -A FORWARD -o wg0 -j ACCEPT; iptables -I FORWARD -s 10.0.0.1/24 -d 10.0.0.1/24 -j DROP; iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

PostDown = iptables -D FORWARD -i wg0 -j ACCEPT; iptables -D FORWARD -o wg0 -j ACCEPT; iptables -D FORWARD -s 10.0.0.1/24 -d 10.0.0.1/24 -j DROP; iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE

ListenPort = 31734

DNS = 100.100.2.136

DNS = 100.100.2.138

##添加一个用户需要对应添加一个peer，填入客户端publickey，ip如10.0.0.2 10.0.0.3

[Peer]

PublicKey = tLg1EJH0bx/UiTclvQN3nM+aTS/72JgjM9O7qX0RKkc=

AllowedIPs = 10.0.0.2/32

## End

## 用户端配置文件模板/etc/wireguard/client.conf（酌情修改）

[Interface]

PrivateKey = IJ4DEmcrwBXngTtDWHBOe9xPsRm9+fbwe8TbpINcXG8=

Address = 10.0.0.2/32

DNS = 100.100.2.136, 100.100.2.138

PersistentKeepalive = 25

[Peer]

PublicKey = +GzVAjpZKkRh5Wz8EJur6a1JAgNGTd85lvPQW4d2AyI=

AllowedIPs = 0.0.0.0/0, ::/0

Endpoint = 1.2.3.4:21734

## End

wg-quick up wg0

wg set wg0 peer $(client1_publickey) allowed-ips 10.0.0.2/32

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

