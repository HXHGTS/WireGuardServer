#include <stdio.h>
#include <stdlib.h>

FILE* server_config, * client_config,*usernum,*bash;
int mode,confirm,ListenPort, num;
char username[10],command[200],pubkey[100],domainname[35],DNS[16];

int DNS_Server(){
    int DNS_choose;
    printf("\n请选择DNS服务器(5、6仅针对服务器在国内的情况):\n\n1.谷歌DNS\n\n2.OpenDNS\n\n3.CloudflareDNS\n\n4.IBM DNS\n\n5.腾讯DNS\n\n6.阿里DNS\n\n7.自定义DNS\n\n请输入:");
    scanf("%d",&DNS_choose);
    if(DNS_choose==1){
        sprintf(DNS, "8.8.8.8");
    }
    else if (DNS_choose == 2) {
        sprintf(DNS, "208.67.222.222");
    }
    else if (DNS_choose == 3) {
        sprintf(DNS, "1.1.1.1");
    }
    else if (DNS_choose == 4) {
        sprintf(DNS, "9.9.9.9");
    }
    else if (DNS_choose == 5) {
        sprintf(DNS, "119.29.29.29");
    }
    else if (DNS_choose == 6) {
        sprintf(DNS, "223.5.5.5");
    }
    else {
        printf("\n请输入DNS服务器地址:");
        scanf("%s", DNS);
    }
    return 0;
}

int main()
{
    UI();
    if (mode == 1) {
        InstallWireGuard();
    }
    else if (mode == 2) {
        AddUser();
    }
    else if (mode == 3) {
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
    printf("-------------当前Kernel版本---------------\n");
    system("uname -a");
    printf("------------------------------------------\n");
    printf("--------WireGuard安装工具(CentOS7)--------\n");
    printf("警告:Kernel版本低于5必须先升级再运行本程序!!!\n\n1.安装WireGuard\n\n2.添加用户\n\n3.关闭WireGuard\n\n4.重启WireGuard\n\n");
    printf("------------------------------------------\n");
    printf("请输入：");
    scanf("%d", &mode);
    if (mode == 1 || mode == 2) {
        DNS_Server();
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
    system("sudo curl -o /etc/yum.repos.d/jdoss-wireguard-epel-7.repo https://copr.fedorainfracloud.org/coprs/jdoss/wireguard/repo/epel-7/jdoss-wireguard-epel-7.repo");
    system("sudo yum install epel-release qrencode -y");
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
    fprintf(server_config, "DNS = %s\n\n",DNS);
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
    printf("\n请输入用户名，必须为英文:");
    scanf("%s", username);
    printf("\n请输入服务器地址:");
    scanf("%s", domainname);
re2:printf("\n请输入服务器监听端口号，与第二步一致:");
    scanf("%d", &ListenPort);
    if (ListenPort < 10000 || ListenPort>65535) {
        printf("\n非法输入，请重新输入端口号！\n");
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
    printf("\n正在计算客户端公钥:\n");
    sprintf(command,"cat /etc/wireguard/%s_publickey",username);
    system(command);
    printf("\n请将上面输出的客户端公钥粘贴至此:\n");
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
    fprintf(client_config, "DNS = %s\n",DNS);
    fprintf(client_config, "\n[Peer]\n");
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
    printf("\n成功添加用户！\n");
    printf("\n电脑版WireGuard客户端建议复制以下内容添加:\n\n");
    sprintf(command, "cat /etc/wireguard/%s.conf", username);
    system(command);
    printf("\n\n手机版WireGuard客户端建议扫描以下二维码添加:\n\n");
    sprintf(command, "qrencode -t ansiutf8 < /etc/wireguard/%s.conf", username);
    system(command);
    return 0;
}
