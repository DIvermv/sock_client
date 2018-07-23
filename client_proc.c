#include "sock_client.h"

int UDP_client(int port, char *message)
{

    int sock;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read, val=1;
    struct ip_mreq Mreq;
    socklen_t addrlen;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    Mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1"); 
    Mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&Mreq,sizeof(Mreq));// отправляем запрос на подписку к адресу
    perror("setsock");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("224.0.0.1");
   // addr.sin_addr.s_addr = htonl(INADDR_ANY);
     if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)// биндим по мультикастовому адресу
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
