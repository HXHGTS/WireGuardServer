#include <stdio.h>
#include <stdlib.h> 

FILE* server_config, * client_config,*usernum,*client_pubkey,*server_info;
int mode,confirm,ListenPort, num;
char username[10],command[200],pubkey[46],ServerName[35],DNS_Reslover[16];
int ret;
char FileName[36];
int DNS_choose;

int DNS_X(){
        printf("\n服务器DNS地址:\n");
        system("cat /etc/resolv.conf");
        printf("\n请选择DNS服务器(不知道怎么选就选2，5、6仅针对服务器在国内的情况):\n\n1.谷歌DNS\n\n2.OpenDNS\n\n3.CloudflareDNS\n\n4.IBM DNS\n\n5.腾讯DNS\n\n6.阿里DNS\n\n7.自定义DNS\n\n请输入:");
        scanf("%d", &DNS_choose);
        if (DNS_choose == 1) {
            sprintf(DNS_Reslover, "8.8.8.8");
        }
        else if (DNS_choose == 2) {
            sprintf(DNS_Reslover, "208.67.222.222");
        }
        else if (DNS_choose == 3) {
            sprintf(DNS_Reslover, "1.1.1.1");
        }
        else if (DNS_choose == 4) {
            sprintf(DNS_Reslover, "9.9.9.9");
        }
        else if (DNS_choose == 5) {
            sprintf(DNS_Reslover, "119.29.29.29");
        }
        else if (DNS_choose == 6) {
            sprintf(DNS_Reslover, "223.5.5.5");
        }
        else {
            printf("\n请输入DNS服务器地址:");
            scanf("%s", DNS_Reslover);
        }
        server_info = fopen("/etc/wireguard/dns.info", "w");
        fprintf(server_info, "%s", DNS_Reslover);
        fclose(server_info);
        system("clear");
    return 0;
}

int main()
{
Menu:UI();
    system("clear");
    if (mode == 1) {
        printf("正在升级Kernel. . .\n");
        KernelUpdate();
    }
    else if (mode == 2) {
        DNS_X();
        system("wg-quick down wg0");
        InstallWireGuard();
    }
    else if (mode == 3) {
        AddUser();
    }
    else if (mode == 4) {
        system("wg-quick down wg0");
        printf("已关闭WireGuard!\n");
    }
    else if (mode == 5) {
        system("wg-quick down wg0");
        system("wg-quick up wg0");
        printf("已重启WireGuard!\n");
    }
    else if (mode == 6) {
        system("wg-quick down wg0");
        printf("修改完成后请手动重启WireGuard!\n");
        system("vi /etc/wireguard/wg0.conf");
    }
    else if (mode == 7) {
        printf("请输入用户编号，如user1请输入1:");
        scanf("%d", &num);
        sprintf(command,"vi /etc/wireguard/user%d.conf",num);
        system(command);
    }
    else {
        system("clear");
        goto Menu;
    }
    return 0;
}

int UI() {
    system("clear");
    printf("----------WireGuard安装工具(CentOS7)----------\n");
    printf("---------------当前Kernel版本-----------------\n");
    system("uname -sr");
    printf("----------------------------------------------\n");
    printf("警告:Kernel版本低于5必须先升级再运行本程序!!!\n1.CentOS7内核升级（版本低于5必须升级，会触发重启！）\n2.安装WireGuard\n3.添加用户\n4.关闭WireGuard\n5.重启WireGuard\n6.修改服务器配置\n7.修改用户配置\n");
    printf("----------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int InstallWireGuard(){
    re1:printf("\n请输入服务器ip地址 端口号,如127.0.0.1 1080 :");
    scanf("%s %d", ServerName, &ListenPort);
    server_info = fopen("/etc/wireguard/servername.info", "w");
    fprintf(server_info, "%s", ServerName);
    fclose(server_info);
    server_info = fopen("/etc/wireguard/port.info", "w");
    fprintf(server_info, "%d", ListenPort);
    fclose(server_info);
    if (ListenPort < 10000 || ListenPort>65535) {
        printf("非法输入，请重新输入端口号！\n");
        goto re1;
    }
    system("clear");
    system("curl -o /etc/yum.repos.d/jdoss-wireguard-epel-7.repo https://copr.fedorainfracloud.org/coprs/jdoss/wireguard/repo/epel-7/jdoss-wireguard-epel-7.repo");
    system("yum install epel-release qrencode -y");
    system("yum install wireguard-dkms wireguard-tools -y");
    if (system("grep \"net.ipv4.ip_forward = 1\" /etc/sysctl.conf") != 0) {
        system("echo \"net.ipv4.ip_forward = 1\" >> /etc/sysctl.conf");
    }
    system("sysctl -p");
    system("rm -rf /etc/wireguard");
    system("mkdir -p /etc/wireguard");
    system("chmod 0777 /etc/wireguard");
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
    fprintf(server_config, "DNS = %s\n", DNS_Reslover);
    fclose(server_config);
    system("systemctl enable wg-quick@wg0");
    printf("服务器搭建完成！\n");
    return 0;
}

int AddUser() {
    if (fopen("/etc/wireguard/user1.conf", "r") == NULL) {
        num = 2;
    }
    else {
        for (ret = 1; ret <= 251; ret++) {
            sprintf(command,"[ -f /etc/wireguard/user%d.conf ]",ret);
            if (system(command) != 0&&ret<251) {
                num = ret+1;
                break;
            }
            if (ret == 251) {
                printf("\n已超过最大人数限制!\n");
                exit(0);
            }
        }
    }
    sprintf(username, "user%d", num - 1);
    server_info = fopen("/etc/wireguard/servername.info", "r");
    fscanf(server_info, "%s", ServerName);
    fclose(server_info);
    server_info = fopen("/etc/wireguard/port.info", "r");
    fscanf(server_info, "%d", &ListenPort);
    fclose(server_info);
    server_info = fopen("/etc/wireguard/dns.info", "r");
    fscanf(server_info, "%s", DNS_Reslover);
    fclose(server_info);
    system("clear");
    sprintf(command, "wg genkey | tee /etc/wireguard/%s_privatekey | wg pubkey > /etc/wireguard/%s_publickey",username,username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "\n[Peer]\n");
    fprintf(server_config, "PublicKey = ");
    fclose(server_config);
    sprintf(command,"cat /etc/wireguard/%s_publickey >> /etc/wireguard/wg0.conf",username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "AllowedIPs = 10.0.0.%d/32\n",num);
    fclose(server_config);
    system("wg-quick down wg0");
    system("wg-quick up wg0");
    sprintf(FileName, "/etc/wireguard/%s_publickey", username);
    client_pubkey = fopen(FileName, "r");
    fgets(pubkey,45,client_pubkey);
    fclose(client_pubkey);
    sprintf(FileName, "/etc/wireguard/%s.conf", username);
    client_config = fopen(FileName, "w");
    fprintf(client_config, "##Can be found in /etc/wireguard/%s.conf\n",username,username);
    fprintf(client_config, "[Interface]\n");
    fprintf(client_config, "PrivateKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/%s_privatekey >> /etc/wireguard/%s.conf", username,username);
    system(command);
    client_config = fopen(FileName, "a");
    fprintf(client_config, "Address = 10.0.0.%d/32\n",num);
    fprintf(client_config, "DNS = %s\n", DNS_Reslover);
    fprintf(client_config, "\n[Peer]\n");
    fprintf(client_config, "AllowedIPs = 0.0.0.0/0, ::/0\n");
    fprintf(client_config, "Endpoint = %s:%d\n",ServerName,ListenPort);
    fprintf(client_config, "PublicKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/server_publickey >> /etc/wireguard/%s.conf", username);
    system(command);
    client_config = fopen(FileName, "a");
    fclose(client_config);
    sprintf(command, "rm -f /etc/wireguard/%s_privatekey", username);
    system(command);
    sprintf(command, "rm -f /etc/wireguard/%s_publickey", username);
    system(command);
    printf("\n成功添加用户！\n");
    printf("\n电脑版WireGuard客户端建议复制以下内容添加:\n\n");
    sprintf(command, "cat /etc/wireguard/%s.conf", username);
    system(command);
    printf("\n\n手机版WireGuard客户端建议扫描以下二维码添加:\n\n");
    sprintf(command, "qrencode -t ansiutf8 < /etc/wireguard/%s.conf", username);
    system(command);
    printf("\n生成的配置文件请不要在本机上改名或删除，如确实需要，请删除文件中内容，避免修改文件名!\n");
    return 0;
}

int KernelUpdate() {
    system("yum install -y wget");
    system("echo \"151.101.108.133 raw.githubusercontent.com\" >> /etc/hosts");
    system("echo \"52.78.231.108 github.com\" >> /etc/hosts");
    system("wget https://github.com/HXHGTS/WireGuardServer/raw/master/preload.sh");
    system("chmod +x preload.sh");
    system("bash preload.sh");
    printf("升级完成，正在重启服务器以应用配置. . .\n");
    system("reboot");
    return 0;
}