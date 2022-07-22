#!/bin/sh

echo '92.243.18.11 www.netfilter.org' > /etc/hosts

echo 正在关闭防火墙. . .

systemctl stop firewalld

systemctl stop ufw

systemctl disable firewalld

systemctl disable ufw

echo 正在安装编译所需依赖. . .

yum install gcc gcc-c++ autoconf autogen -y

yum install libtool libtool-ltdl libtool-ltdl-devel bzip2 git unzip -y

yum install centos-release-scl -y

yum install devtoolset-8-gcc* -y

yum install iptables-services iptables-persistent -y

source /opt/rh/devtoolset-8/enable

echo 正在下载项目文件. . .

cd root

#原版fullcone NAT https://github.com/Chion82/netfilter-full-cone-nat/archive/refs/heads/master.zip
git clone https://github.com/HXHGTS/netfilter-full-cone-nat/ netfilter-full-cone-nat
#加入ipv6支持

wget https://www.netfilter.org/pub/iptables/iptables-1.8.7.tar.bz2 -O iptables.tar.bz2

wget https://www.netfilter.org/pub/libnftnl/libnftnl-1.2.0.tar.bz2 -O libnftnl.tar.bz2

wget https://www.netfilter.org/pub/libmnl/libmnl-1.0.4.tar.bz2 -O libmnl.tar.bz2

tar -jxvf libmnl.tar.bz2

tar -jxvf iptables.tar.bz2

tar -jxvf libnftnl.tar.bz2

cp -rf libmnl-1.0.4 libmnl

cp -rf iptables-1.8.7 iptables

cp -rf libnftnl-1.2.0 libnftnl

rm -rf libmnl-1.0.4 iptables-1.8.7 libnftnl-1.2.0

echo 开始编译libmnl. . .

cd /root/libmnl

./configure

autoreconf -ivf

make

make install

echo 开始编译libnftnl. . .

cd /root/libnftnl

PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

export PKG_CONFIG_PATH

./configure

autoreconf -ivf

make

make install

echo 开始编译netfilter-full-cone-nat. . .

cd /root/netfilter-full-cone-nat

make

modprobe nf_nat

insmod xt_FULLCONENAT.ko

cp -f /root/netfilter-full-cone-nat/libipt_FULLCONENAT.c /root/iptables/extensions/

cp -f /root/netfilter-full-cone-nat/libip6t_FULLCONENAT.c /root/iptables/extensions/

echo 开始编译iptables. . .

cd /root/iptables

ln -sfv /usr/sbin/xtables-multi /usr/bin/iptables-xml

./autogen.sh

./configure

autoreconf -ivf

make

make install

echo 正在替换iptables. . .

systemctl stop iptables

rm -rf /sbin/iptables

rm -rf /sbin/iptables-restore

rm -rf /sbin/iptables-save

cd /usr/local/sbin

cp -f /usr/local/sbin/iptables /sbin/

cp -f /usr/local/sbin/iptables-restore /sbin/

cp -f /usr/local/sbin/iptables-save /sbin/

kernel=`uname -r`

cp -f /root/netfilter-full-cone-nat/xt_FULLCONENAT.ko  /lib/modules/$kernel/

depmod

echo "modprobe xt_FULLCONENAT" > /etc/sysconfig/modules/xt_FULLCONENAT.modules

chmod 755 /etc/sysconfig/modules/xt_FULLCONENAT.modules

rm -rf /root/iptables.tar.bz2 /root/libmnl.tar.bz2 /root/libnftnl.tar.bz2 

echo 下方有输出则安装完成:

echo -----------------------------------------------------

lsmod | grep xt_FULLCONENAT

echo -----------------------------------------------------

exit 0
