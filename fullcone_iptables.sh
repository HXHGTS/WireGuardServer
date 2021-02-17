#!/bin/sh

echo 正在安装编译所需依赖. . .

yum install gcc gcc-c++ autoconf autogen libmnl libmnl-devel libtool-devel libtool git -y

yum install centos-release-scl -y

yum install devtoolset-8-gcc* -y

yum install libmnl-devel -y

scl enable devtoolset-8 bash

echo 正在克隆项目文件. . .

cd /root/

git clone git://git.netfilter.org/libnftnl.git

git clone git://git.netfilter.org/iptables.git

git clone https://github.com/Chion82/netfilter-full-cone-nat.git

echo 开始编译libnftnl. . .

cd libnftnl

sh autogen.sh

./configure

make

make install

echo 开始编译iptables. . .

cd /root/netfilter-full-cone-nat

make

modprobe nf_nat

insmod xt_FULLCONENAT.ko

cp /root/netfilter-full-cone-nat/libipt_FULLCONENAT.c /root/iptables/extensions/

cd /root/iptables

ln -sfv /usr/sbin/xtables-multi /usr/bin/iptables-xml

./autogen.sh

PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

export PKG_CONFIG_PATH

./configure --disable-nftables

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

iptables -V

kernel=`uname -r`

cp /root/netfilter-full-cone-nat/xt_FULLCONENAT.ko  /lib/modules/$kernel/

depmod

echo "modprobe xt_FULLCONENAT" > /etc/sysconfig/modules/xt_FULLCONENAT.modules

chmod 755 /etc/sysconfig/modules/xt_FULLCONENAT.modules

echo 下方有输出则安装完成:

echo -----------------------------------------------------

lsmod | grep xt_FULLCONENAT

echo -----------------------------------------------------

exit 0
