#!/bin/sh

echo 正在安装编译所需依赖. . .

yum install gcc-c++ autoconf autogen -y

yum install libtool libtool-ltdl libtool-ltdl-devel bzip2 unzip -y

yum install centos-release-scl -y

yum install devtoolset-8-gcc* -y

echo 正在下载项目文件. . .

rm -rf /root/libmnl

rm -rf /root/iptables

rm -rf /root/libnftnl

rm -rf /root/netfilter-full-cone-nat

wget https://github.com/Chion82/netfilter-full-cone-nat/archive/master.zip -O netfilter-full-cone-nat.zip

wget https://www.netfilter.org/pub/iptables/iptables-1.8.7.tar.bz2 -O iptables.tar.bz2

wget https://www.netfilter.org/pub/libnftnl/libnftnl-1.1.9.tar.bz2 -O libnftnl.tar.bz2

wget https://www.netfilter.org/pub/libmnl/libmnl-1.0.4.tar.bz2 -O libmnl.tar.bz2

tar -jxvf libmnl.tar.bz2

sleep 3

tar -jxvf iptables.tar.bz2

sleep 3

unzip netfilter-full-cone-nat.zip

sleep 3

tar -jxvf libnftnl.tar.bz2

sleep 3

cp -rf libmnl-1.0.4 libmnl

cp -rf iptables-1.8.7 iptables

cp -rf libnftnl-1.1.9 libnftnl

cp -rf netfilter-full-cone-nat-master netfilter-full-cone-nat

rm -rf libmnl-1.0.4

rm -rf iptables-1.8.7

rm -rf libnftnl-1.1.9

rm -rf netfilter-full-cone-nat-master

sleep 3

echo 开始编译libmnl. . .

cd /root/libmnl

./configure

sleep 3

autoreconf -ivf

make

make install

sleep 3

echo 开始编译libnftnl. . .

cd /root/libnftnl

PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

export PKG_CONFIG_PATH

./configure

sleep 3

autoreconf -ivf

make

make install

sleep 3

echo 开始编译iptables. . .

cd /root/netfilter-full-cone-nat

make

sleep 3

modprobe nf_nat

insmod xt_FULLCONENAT.ko

cp -f /root/netfilter-full-cone-nat/libipt_FULLCONENAT.c /root/iptables/extensions/

cd /root/iptables

ln -sfv /usr/sbin/xtables-multi /usr/bin/iptables-xml

./autogen.sh

sleep 3

./configure

sleep 3

make

make install

sleep 3

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

cp -f /root/netfilter-full-cone-nat/xt_FULLCONENAT.ko  /lib/modules/$kernel/

depmod

echo "modprobe xt_FULLCONENAT" > /etc/sysconfig/modules/xt_FULLCONENAT.modules

chmod 755 /etc/sysconfig/modules/xt_FULLCONENAT.modules

rm -rf /root/libmnl

rm -rf /root/iptables

rm -rf /root/libnftnl

rm -rf /root/netfilter-full-cone-nat

echo 下方有输出则安装完成:

echo -----------------------------------------------------

lsmod | grep xt_FULLCONENAT

echo -----------------------------------------------------

exit 0
