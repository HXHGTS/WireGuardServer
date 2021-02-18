### 国内服务器直接运行(仅支持CentOS7):

`yum install -y gcc wget && curl https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/hosts > /etc/hosts && wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`

### 国内服务器直接运行(支持v4/v6双栈网络)(仅支持CentOS7):

`yum install -y gcc wget && curl https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/hosts > /etc/hosts && wget https://cdn.jsdelivr.net/gh/HXHGTS/WireGuardServer/WireGuard_ipv6.c -O WireGuard.c && chmod +x WireGuard.c && gcc -o WireGuard WireGuard.c && chmod +x WireGuard && ./WireGuard`
