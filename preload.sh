#!/bin/bash

yum -y install epel-release curl vim wget

sed -i "0,/enabled=0/s//enabled=1/" /etc/yum.repos.d/epel.repo

yum remove -y kernel-devel

rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org

rpm -Uvh https://mirrors.aliyun.com/centos/8.2.2004/extras/ppc64le/os/Packages/elrepo-release-8.1-1.el8.elrepo.noarch.rpm?spm=a2c6h.13651111.0.0.2d3b2f70SDs6ec&file=elrepo-release-8.1-1.el8.elrepo.noarch.rpm

yum --disablerepo="*" --enablerepo="elrepo-kernel" list available

yum -y --enablerepo=elrepo-kernel install kernel-ml

sed -i "s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/" /etc/default/grub

grub2-mkconfig -o /boot/grub2/grub.cfg

wget https://mirrors.aliyun.com/centos/7.8.2003/extras/x86_64/Packages/elrepo-release-7.0-4.el7.elrepo.noarch.rpm?spm=a2c6h.13651111.0.0.2d3b2f70SDs6ec&file=elrepo-release-7.0-4.el7.elrepo.noarch.rpm

rpm -ivh elrepo-release-7.0-4.el7.elrepo.noarch.rpm

yum -y --enablerepo=elrepo-kernel install kernel-ml-devel

reboot
