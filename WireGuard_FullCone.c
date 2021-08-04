#include <stdio.h>
#include <stdlib.h> 

FILE* server_config, * client_config,*usernum,*client_pubkey,*server_info;
int mode,confirm,ListenPort, num;
char username[10],command[200],pubkey[46],ServerName[35];
int ret,os_version;
char FileName[36];

int DNS() {
    system("mkdir -p /etc/wireguard");
    system("umask 077 /etc/wireguard");
    system("yum install bind-utils dnsmasq -y");
    printf("正在配置DNS. . .\n");
    server_info = fopen("/etc/dnsmasq.conf", "w");
    fprintf(server_info, "resolv-file=/etc/resolv.dnsmasq.conf\n");
    fprintf(server_info, "strict-order\n");
    fprintf(server_info, "addn-hosts=/etc/dnsmasq.hosts\n");
    fclose(server_info);//使用系统默认DNS解析
    system("cat /etc/resolv.conf | grep nameserver > /etc/resolv.dnsmasq.conf");
    server_info = fopen("/etc/dnsmasq.hosts", "w");
    fprintf(server_info, "addn-hosts=/etc/dnsmasq.hosts\n");
    fclose(server_info);
    system("systemctl start dnsmasq");
    system("systemctl enable dnsmasq");
    system("clear");
    return 0;
}

int main()
{
Menu:UI();
    system("clear");
    if (mode == 1) {
        KernelUpdate();
        DNS();
        InstallWireGuard();
    }
    else if (mode == 2) {
        AddUser();
    }
    else if (mode == 3) {
        system("wg-quick down wg0");
        printf("已关闭WireGuard!\n");
    }
    else if (mode == 4) {
        system("wg-quick down wg0");
        system("wg-quick up wg0");
        printf("已重启WireGuard!\n");
    }
    else if (mode == 5) {
        system("clear");
        printf("服务器信息如下:\n");
        system("wg");
    }
    else if (mode == 6) {
        system("wg-quick down wg0");
        printf("正在打开配置文件. . .\n");
        system("vi /etc/wireguard/wg0.conf");
        system("wg-quick down wg0");
        system("wg-quick up wg0");
        printf("修改完成!\n");
    }
    else if (mode == 7) {
        printf("请输入用户编号，如user1请输入1:");
        scanf("%d", &num);
        sprintf(command,"vi /etc/wireguard/user%d.conf",num);
        system(command);
        printf("\n成功修改用户！\n");
        system("wg-quick down wg0");
        system("wg-quick up wg0");
        printf("\n\n手机版WireGuard客户端建议扫描以下二维码添加:\n\n");
        sprintf(command, "qrencode -t ansiutf8 < /etc/wireguard/user%d.conf", num);
        system(command);
        system("sleep 1");
        printf("\n电脑版WireGuard客户端建议复制以下内容添加:\n\n");
        sprintf(command, "cat /etc/wireguard/user%d.conf", num);
        system(command);
        printf("\n生成的配置文件请不要在本机上改名或删除，如确实需要，请删除文件中内容，避免修改文件名!\n");
        system("sleep 1");
    }
    else if (mode == 8) {
        system("wg-quick down wg0");
        system("yum remove -y wireguard-dkms wireguard-tools");
        system("rm -rf /etc/wireguard");
        system("rm -rf /root/preload.sh");
        printf("已销毁服务器!!!\n");
    }
    else {
        exit(0);
    }
    goto Menu;
}

int UI() {
    printf("----------WireGuard安装工具(CentOS7)----------\n");
    printf("---------------当前Kernel版本-----------------\n");
    system("uname -sr");
    printf("----------------------------------------------\n");
    printf("警告:Kernel版本低于5运行模式1会自动升级内核并重启，重启后点击键盘方向键上并回车选择模式1以继续执行代码\n1.安装或重装WireGuard(重装前必须先销毁服务器)\n2.添加用户\n3.关闭WireGuard\n4.重启WireGuard\n5.查看服务器信息\n6.修改服务器配置\n7.修改用户配置\n8.销毁服务器(用于重新配置服务器)\n0.退出\n");
    printf("----------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int InstallWireGuard(){
    system("clear");
    re1:printf("请输入服务器端口号,建议10000-60000,如10800:");
    scanf("%d",&ListenPort);
    server_info = fopen("/etc/wireguard/port.info", "w");
    fprintf(server_info, "%d", ListenPort);
    fclose(server_info);
    if (ListenPort < 23 || ListenPort>65535) {
        printf("非法输入，请重新输入端口号！\n");
        goto re1;
    }
    printf("正在检测本机ip地址，请稍后. . . . . .\n");
    system("yum install curl -y");
    system("curl -s ifconfig.me/ip > /etc/wireguard/servername.info");
    system("clear");
    printf("正在安装WireGuard. . . . . .\n");
    system("yum install iptables-services -y");
    system("curl -o /etc/yum.repos.d/jdoss-wireguard-epel-7.repo https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/jdoss-wireguard-epel-7.repo");
    system("yum install epel-release wireguard-dkms wireguard-tools qrencode -y");
    printf("正在编译安装fullcone iptables. . . . . .\n");
    system("curl -sSL https://raw.githubusercontent.com/HXHGTS/WireGuardServer/master/fullcone_iptables.sh | sh");
    printf("正在生成服务器配置. . . . . .\n");
    server_config = fopen("/etc/wireguard/wg0.conf", "w");
    fprintf(server_config, "[Interface]\n");
    fprintf(server_config, "PrivateKey = ");
    fclose(server_config);
    system("wg genkey | tee /etc/wireguard/server_privatekey | wg pubkey > /etc/wireguard/server_publickey");
    system("cat /etc/wireguard/server_privatekey >> /etc/wireguard/wg0.conf");//服务器私钥，不要修改
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "Address = 10.0.0.1/32\n");//服务器ip地址，修改需要同时修改客户端配置
    fprintf(server_config, "PostUp = iptables -t nat -A POSTROUTING -o eth0 -j FULLCONENAT; iptables -t nat -A PREROUTING -i eth0 -j FULLCONENAT\n");//服务器防火墙配置
    fprintf(server_config, "PostDown = iptables -F; iptables -Z\n");//服务器防火墙配置
    fprintf(server_config, "ListenPort = %d\n",ListenPort);//服务器监听端口
    fclose(server_config);
    system("rm -f /etc/wireguard/server_privatekey");
    printf("正在启动服务器. . . . . .\n");
    system("systemctl enable wg-quick@wg0");
    printf("服务器搭建完成！\n");
    printf("正在默认添加用户1. . .\n");
    AddUser();
    printf("需要添加更多用户请使用\"添加用户\"功能!\n");
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
                printf("\n已超过最大人数限制!\n");//未来更新可能会修改此处限制，但250台设备限制暂时够用
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
    system("clear");
    system("wg genpsk > /etc/wireguard/psk"); 
    sprintf(command, "wg genkey | tee /etc/wireguard/%s_privatekey | wg pubkey > /etc/wireguard/%s_publickey",username,username);
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "\n[Peer]\n");
    fprintf(server_config, "PublicKey = ");
    fclose(server_config);
    sprintf(command,"cat /etc/wireguard/%s_publickey >> /etc/wireguard/wg0.conf",username);//客户端公钥，不要修改
    system(command);
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "AllowedIPs = 10.0.0.%d/32\n",num);//客户端ip地址分配，不要修改
    fprintf(server_config, "PresharedKey = ");
    fclose(server_config); 
    system("cat /etc/wireguard/psk >> /etc/wireguard/wg0.conf");//预共享密钥，不要修改
    server_config = fopen("/etc/wireguard/wg0.conf", "a");
    fprintf(server_config, "\n");
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
    fprintf(client_config, "DNS = 10.0.0.1\n");
    fprintf(client_config, "\n[Peer]\n");
    fprintf(client_config, "AllowedIPs = 0.0.0.0/0\n");
    fprintf(client_config, "Endpoint = %s:%d\n",ServerName,ListenPort);
    fprintf(client_config, "PublicKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/server_publickey >> /etc/wireguard/%s.conf", username);
    system(command);
    client_config = fopen(FileName, "a");
    fprintf(client_config, "PresharedKey = ");
    fclose(client_config);
    sprintf(command, "cat /etc/wireguard/psk >> /etc/wireguard/%s.conf",username);
    system(command);
    client_config = fopen(FileName, "a");
    fprintf(client_config, "\n");
    fclose(client_config); 
    sprintf(command, "rm -f /etc/wireguard/%s_privatekey", username);
    system(command);
    sprintf(command, "rm -f /etc/wireguard/%s_publickey", username);
    system(command);
    system("rm -f /etc/wireguard/psk");
    printf("\n成功添加用户！\n");
    printf("\n电脑版WireGuard客户端建议复制以下内容添加:\n\n");
    sprintf(command, "cat /etc/wireguard/%s.conf", username);
    system(command);
    printf("\n\n手机版WireGuard客户端建议扫描以下二维码添加:\n\n");
    sprintf(command, "qrencode -t ansiutf8 < /etc/wireguard/%s.conf", username);
    system(command);
    printf("\n生成的配置文件请不要在本机上改名或删除，如确实需要，请删除文件中内容，避免修改文件名!\n");
    system("sleep 1");
    return 0;
}

int KernelUpdate() {
    if ((fopen("KernelUpdate.sh", "r")) == NULL) {
        system("cat /etc/redhat-release|sed -r 's/.* ([0-9]+)\\..*/\\1/' > os_version.info");
        server_config=fopen("os_version.info","r");
        fscanf(server_config,"%d",&os_version);
        fclose(server_config);
        printf("正在升级新内核. . .\n");
        if(os_version==7){system("wget https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/KernelUpdate.sh -O KernelUpdate.sh");}
        else if(os_version==8){system("wget https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/KernelUpdate8.sh -O KernelUpdate.sh");}
        else{printf("不支持的操作系统版本!请更换操作系统为CentOS7/8后再试!");exit(0);}
        system("chmod +x KernelUpdate.sh");
        printf("正在升级，将自动触发重启以应用配置. . .\n");
        system("bash KernelUpdate.sh");
    }
    else {
        system("curl -sSL https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/TCPO.sh | sh");
    }
    return 0;
}
