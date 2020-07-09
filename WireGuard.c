#include <stdio.h>
#include <stdlib.h>

FILE* server_config, * client_config,*usernum,*bash;
int mode,confirm,ListenPort, num;
char username[10],command[200],pubkey[100],domainname[30];

int main()
{
    UI();
    if (mode == 1) {
        UpdateKernel();
    }
    else if (mode == 2) {
        InstallWireGuard();
    }
    else if (mode == 3) {
        AddUser();
    }
    else if (mode == 4) {
        system("sudo wg-quick down wg0");
        printf("已关闭WireGuard!\n");
    }
    else {
        system("sudo wg-quick down wg0");
        system("sudo wg-quick up wg0");
        printf("已重启WireGuard!\n");
    }
    return 0;
}

int UI() {
    printf("--------WireGuard安装工具(CentOS7)--------\n\n");
    printf("1.更新CentOS7内核（必须）\n\n2.安装WireGuard\n\n3.添加wg用户\n\n4.关闭WireGuard\n\n5.重启WireGuard\n\n请输入：");
    scanf("%d", &mode);
    return 0;
}

int UpdateKernel() {
    bash = fopen("KernelUpdate.sh", "w");
    fprintf(bash,"sudo yum -y install epel-release curl vim wget");
    fprintf(bash,"sudo sed -i \"0,/enabled=0/s//enabled=1/\" /etc/yum.repos.d/epel.repo");
    fprintf(bash,"sudo yum remove -y kernel-devel");
    fprintf(bash,"sudo rpm –import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org");
    fprintf(bash,"sudo rpm -Uvh https://github.com/HXHGTS/WireGuardServer/raw/master/elrepo-release-7.0-5.el7.elrepo.noarch.rpm");
    fprintf(bash,"sudo yum –disablerepo=\"*\" –enablerepo=\"elrepo-kernel\" list available");
    fprintf(bash,"sudo yum -y –enablerepo=elrepo-kernel install kernel-ml");
    fprintf(bash,"sudo sed -i \"s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/\" /etc/default/grub");
    fprintf(bash,"sudo grub2-mkconfig -o /boot/grub2/grub.cfg");
    fprintf(bash,"wget https://github.com/HXHGTS/WireGuardServer/raw/master/kernel-ml-devel-5.7.8-1.el7.elrepo.x86_64.rpm");
    fprintf(bash,"sudo rpm -ivh kernel-ml-devel-5.7.8-1.el7.elrepo.x86_64.rpm");
    fprintf(bash,"sudo yum -y –enablerepo=elrepo-kernel install kernel-ml-devel");
    fclose(bash);
    system("KernelUpdate.sh");
    printf("内核升级准备就绪，请自行重启计算机. . .\n");
    return 0;
}

int InstallWireGuard(){
    re1:printf("请输入服务器监听端口号(10000-65535):");
    scanf("%d", &ListenPort);
    if (ListenPort < 10000 || ListenPort>65535) {
        printf("非法输入，请重新输入端口号！\n");
        goto re1;
    }
    system("curl -Lo /etc/yum.repos.d/wireguard.repo https://copr.fedorainfracloud.org/coprs/jdoss/wireguard/repo/epel-7/jdoss-wireguard-epel-7.repo");
    system("sudo yum install epel-release -y");
    system("sudo yum install wireguard-dkms wireguard-tools -y");
    system("sudo echo \"net.ipv4.ip_forward = 1\" >> /etc/sysctl.conf");
    system("sudo sysctl -p");
    system("sudo mkdir -p /etc/wireguard");
    system("sudo chmod 0777 /etc/wireguard");
    server_config = fopen("/etc/wireguard/wg0.conf", "w");
    fprintf(server_config, "[Interface]\n");
    fprintf(server_config, "PrivateKey = ");
    fclose(server_config);
    system("wg genkey | tee /etc/wireguard/server_privatekey | wg pubkey > /etc/wireguard/server_publickey");
    system("cat /etc/wireguard/server_privatekey >> /etc/wireguard/wg0.conf");
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "Address = 10.0.0.1/16\n");
    fprintf(server_config, "PostUp = iptables -A FORWARD -i wg0 -j ACCEPT; iptables -A FORWARD -o wg0 -j ACCEPT; iptables -I FORWARD -s 10.0.0.1/24 -d 10.0.0.1/24 -j DROP; iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE\n");
    fprintf(server_config, "PostDown = iptables -D FORWARD -i wg0 -j ACCEPT; iptables -D FORWARD -o wg0 -j ACCEPT; iptables -D FORWARD -s 10.0.0.1/24 -d 10.0.0.1/24 -j DROP; iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE\n");
    fprintf(server_config, "ListenPort = %d\n",ListenPort);
    fprintf(server_config, "DNS = 8.8.8.8\n");
    fprintf(server_config, "DNS = 8.8.4.4\n\n");
    fclose(server_config);
    printf("服务器搭建完成！\n");
    return 0;
}

int AddUser() {
    if (fopen("/etc/wireguard/usernum.conf", "r") == NULL) {
        usernum = fopen("/etc/wireguard/usernum.conf", "w");
        fprintf(usernum, "%d", 2);
        fclose(usernum);
        num = 2;
    }
    else {
        printf("检测到当前用户数为:\n");
        system("cat /etc/wireguard/usernum.conf");
        re3:printf("\n请输入新建用户编号，必须等于上值，否则会出错：");
        scanf("%d", &num);
        if (num <= 2) {
            printf("非法输入，请重新输入用户编号！\n");
            goto re3;
        }
    }
    printf("请输入用户名，必须为英文:");
    scanf("%s", username);
    printf("请输入服务器地址:");
    scanf("%s", domainname);
re2:printf("请输入服务器监听端口号，与第二部一致:");
    scanf("%d", &ListenPort);
    if (ListenPort < 10000 || ListenPort>65535) {
        printf("非法输入，请重新输入端口号！\n");
        goto re2;
    }
    printf("\n");
    sprintf(command, "wg genkey | tee /etc/wireguard/%s_privatekey | wg pubkey > /etc/wireguard/%s_publickey",username,username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "[Peer]\n");
    fprintf(server_config, "PublicKey = ");
    fclose(server_config);
    sprintf(command,"cat /etc/wireguard/%s_publickey >> /etc/wireguard/wg0.conf",username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "AllowedIPs = 10.0.0.%d/32\n",num);
    fclose(server_config);
    system("wg-quick up wg0");
    printf("正在计算客户端公钥:\n");
    sprintf(command,"cat /etc/wireguard/%s_publickey",username);
    system(command);
    printf("请将上面输出的客户端公钥粘贴至此:\n");
    scanf("%s", pubkey);
    sprintf(command,"wg set wg0 peer %s allowed-ips 10.0.0.%d/32",pubkey,num);
    system(command);
    system("sudo systemctl enable wg-quick@wg0");
    client_config = fopen("/etc/wireguard/client.conf", "w");
    fprintf(client_config, "[Interface]\n");
    fprintf(client_config, "PrivateKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/%s_privatekey >> /etc/wireguard/client.conf", username);
    system(command);
    client_config = fopen("/etc/wireguard/client.conf", "a");
    fprintf(client_config, "Address = 10.0.0.%d/32\n",num);
    fprintf(client_config, "DNS = 8.8.8.8, 8.8.4.4\n");
    fprintf(client_config, "[Peer]\n");
    fprintf(client_config, "AllowedIPs = 0.0.0.0/0, ::/0\n");
    fprintf(client_config, "Endpoint = %s:%d\n",domainname,ListenPort);
    fprintf(client_config, "PublicKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/server_publickey >> /etc/wireguard/client.conf", username);
    system(command);
    client_config = fopen("/etc/wireguard/client.conf", "a");
    fclose(client_config);
    sprintf(command, "sudo cp /etc/wireguard/client.conf /etc/wireguard/%s.conf", username);
    system(command);
    system("sudo rm -f /etc/wireguard/client.conf");
    usernum = fopen("/etc/wireguard/usernum.conf", "w");
    fprintf(usernum, "%d", num + 1);
    fclose(usernum);
    printf("成功添加用户！\n");
    return 0;
}