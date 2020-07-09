#include <stdio.h>
#include <stdlib.h>

FILE* server_config, * client_config,*usernum;
int mode,confirm,ListenPort,num;
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
    else {
        AddUser();
    }
    return 0;
}

int UI() {
    printf("--------WireGuard安装工具(CentOS7)--------\n\n");
    printf("1.更新CentOS7内核（必须）\n\n2.安装WireGuard\n\n3.添加wg用户\n\n请输入：");
    scanf("%d", &mode);
    return 0;
}

int UpdateKernel() {
    system("yum -y install epel-release curl vim wget");
    system("sed -i \"0,/enabled=0/s//enabled=1/\" /etc/yum.repos.d/epel.repo");
    system("yum remove -y kernel-devel");
    system("rpm –import https://www.elrepo.org/RPM-GPG-KEY-elrepo.org");
    system("rpm -Uvh https://mirror.tuna.tsinghua.edu.cn/elrepo/elrepo/el7/x86_64/RPMS/elrepo-release-7.0-5.el7.elrepo.noarch.rpm");
    system("yum –disablerepo=\"*\" –enablerepo=\"elrepo-kernel\" list available");
    system("yum -y –enablerepo=elrepo-kernel install kernel-ml");
    system("sed -i \"s/GRUB_DEFAULT=saved/GRUB_DEFAULT=0/\" /etc/default/grub");
    system("grub2-mkconfig -o /boot/grub2/grub.cfg");
    system("wget https://mirror.tuna.tsinghua.edu.cn/elrepo/kernel/el7/x86_64/RPMS/kernel-ml-devel-5.7.6-1.el7.elrepo.x86_64.rpm");
    system("rpm -ivh kernel-ml-devel-5.7.6-1.el7.elrepo.x86_64.rpm");
    system("yum -y –enablerepo=elrepo-kernel install kernel-ml-devel");
    printf("内核升级准备就绪，需要重启计算机，立即重启请输入1并回车:");
    scanf("%d", &confirm);
    if (confirm == 1) {
        system("reboot");
    }
    else {
        printf("重启已被取消，请稍后自行重启计算机！\n");
    }
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
    system("yum install epel-release -y");
    system("yum install wireguard-dkms wireguard-tools -y");
    system("echo \"net.ipv4.ip_forward = 1\" >> /etc/sysctl.conf");
    system("sysctl -p");
    system("mkdir -p /etc/wireguard && chmod 0777 /etc/wireguard");
    server_config = fopen("/etc/wireguard/wg0.conf", "w");
    fprintf(server_config, "[Interface]\n");
    fprintf(server_config, "PrivateKey = ");
    fclose(server_config);
    system("wg genkey | tee /etc/wireguard/server_privatekey | wg pubkey > /etc/wireguard/server_publickey");
    system("cat /etc/wireguard/server_privatekey >> /etc/wireguard/wg0.conf");
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "\n");
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
    }
    usernum = fopen("/etc/wireguard/usernum.conf", "r");
    fread(num,1,1,usernum);
    fclose(usernum);
    printf("请输入用户名，必须为英文:");
    scanf("%s", username);
    printf("请输入服务器地址:");
    scanf("%s", domainname);
    sprintf(command, "wg genkey | tee /etc/wireguard/%s_privatekey | wg pubkey > /etc/wireguard/%s_publickey",username,username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "[Peer]\n");
    fprintf(server_config, "PublicKey = ");
    fclose(server_config);
    sprintf(command,"cat /etc/wireguard/%s_publickey >> /etc/wireguard/wg0.conf",username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "\n");
    fprintf(server_config, "AllowedIPs = 10.0.0.%d/32\n",num);
    fclose(server_config);
    system("wg-quick up wg0");
    printf("正在计算pubkey:\n");
    sprintf(command,"cat /etc/wireguard/%s_publickey",username);
    system(command);
    printf("\n");
    printf("请将pubkey粘贴至此:\n");
    scanf("%s", pubkey);
    sprintf(command,"wg set wg0 peer %s allowed-ips 10.0.0.%d/32",pubkey,num);
    system(command);
    system("systemctl enable wg-quick@wg0");
    client_config = fopen("/etc/wireguard/client.conf", "w");
    fprintf(client_config, "[Interface]\n");
    fprintf(client_config, "PrivateKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/%s_privatekey >> /etc/wireguard/client.conf", username);
    system(command);
    client_config = fopen("/etc/wireguard/client.conf", "a");
    fprintf(client_config, "\n");
    fprintf(client_config, "Address = 10.0.0.%d/32\n",num);
    fprintf(client_config, "DNS = 8.8.8.8, 8.8.4.4\n");
    fprintf(client_config, "PersistentKeepalive = 25\n");
    fprintf(client_config, "[Peer]\n");
    fprintf(client_config, "AllowedIPs = 0.0.0.0/0, ::/0\n");
    fprintf(client_config, "Endpoint = %s:%d\n",domainname,ListenPort);
    fprintf(client_config, "PublicKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/server_publickey >> /etc/wireguard/client.conf", username);
    system(command);
    client_config = fopen("/etc/wireguard/client.conf", "a");
    fprintf(client_config, "\n");
    fclose(client_config);
    sprintf(command, "cp /etc/wireguard/client.conf /etc/wireguard/%s.conf", username);
    system(command);
    system("rm -f /etc/wireguard/client.conf");
    usernum = fopen("/etc/wireguard/usernum.conf", "w");
    num++;
    fprintf(usernum, "%d", num);
    fclose(usernum);
    printf("成功添加用户！\n");
    return 0;
}