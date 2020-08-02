#!/bin/bash

yum -y install epel-release

sed -i "0,/enabled=0/s//enabled=1/" /etc/yum.repos.d/epel.repo

yum remove -y kernel-devel

rpm --import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org

rpm -Uvh https://www.elrepo.org/elrepo-release-7.el7.elrepo.noarch.rpm

yum --disablerepo="*" --enablerepo="elrepo-kernel" list available

yum --enablerepo=elrepo-kernel install kernel-ml kernel-ml-devel -y

sed -i "s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/" /etc/default/grub

grub2-mkconfig -o /boot/grub2/grub.cfg

