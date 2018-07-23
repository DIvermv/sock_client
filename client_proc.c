#include "sock_client.h"

int UDP_client(int port, char *message)
{

    int sock;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    socklen_t addrlen;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("192.168.0.255");
     if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)// биндим по бродкастовому адресу
          perror("bind UDP");
    for( ;;)
    {
         bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
        // bytes_read = recv(sock, buf, 1024, 0);
         perror("recvfrom");
      	 buf[bytes_read] = '\0';
         printf("receive:%i  %s\n",bytes_read,buf);
    }
    close(sock);
    return 0;
}
