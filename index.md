# WireGuardServer

## 一、更新内核(Kernel版本低于5必须执行！)

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



