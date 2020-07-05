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

mkdir /etc/wireguard

cd /etc/wireguard

wg genkey | tee privatekey | wg pubkey > publickey

chmod 777 -R /etc/wireguard

vim /etc/wireguard/wg0.conf

## 三、配置文件

[Interface]

Address = 172.27.224.1/24

PrivateKey = qLQwqD6pzZ/+nCEOoTAyJzH2G8WS5dBi+87OwUrmT3Y=

PostUp   = iptables -A FORWARD -i wg0 -j ACCEPT; iptables -A FORWARD -o wg0 -j ACCEPT; iptables -I FORWARD -s 172.27.224.1/24 -d 172.27.224.1/24 -j DROP; iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE

PostDown = iptables -D FORWARD -i wg0 -j ACCEPT; iptables -D FORWARD -o wg0 -j ACCEPT; iptables -D FORWARD -s 172.27.224.1/24 -d 172.27.224.1/24 -j DROP; iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE

ListenPort = 21735

DNS = 67.207.67.2

MTU = 1492

[Peer]

PublicKey = Vluwx6KB+GWt/DZuPGnquwFn2OdQW0qMXzyQMWle01k=

AllowedIPs = 172.27.224.2/32

## 四、启动

wg-quick up wg0

systemctl enable wg-quick@wg0

## 五、用户配置

[Interface]

Address = 172.27.224.2/24

ListenPort = 21735

PrivateKey = <Private Key>

[Peer]

PublicKey = Vluwx6KB+GWt/DZuPGnquwFn2OdQW0qMXzyQMWle01k=

AllowedIPs = 172.27.224.1/32

Endpoint = 服务器端的公网IP:21735

