#include "sock_client.h"

int Raw_client(int port, char *message)
{

    int sock;
    struct sockaddr_in addr;
    struct UdpHead UDP_Header;
    char buf[1024];
    char mes[1016];
    int bytes_read;
    socklen_t addrlen;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    //addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    UDP_Header.dest_port = htons(port);// записываем номер порта получателя
    UDP_Header.src_port = htons(7777);// записываем номер порта отправителя - не принципиален
    for(int i=0 ;i<20;i++)
    {
	   sprintf(mes,"send message %i\n",i);// сформировали сообщение
           UDP_Header.length=sizeof(UDP_Header)+sizeof(mes);// посчитали размер нагрузки
           memcpy(&buf,&UDP_Header,sizeof(UDP_Header));
	   memcpy((&buf+sizeof(UDP_Header)),mes,sizeof(mes));
	    sendto(sock, buf, sizeof(UDP_Header)+sizeof(mes), 0, (struct sockaddr *)&addr, sizeof(addr));
         bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
        // bytes_read = recv(sock, buf, 1024, 0);
         perror("recvfrom");
      	 buf[bytes_read] = '\0';
         printf("receive:%i  %s\n",bytes_read,buf);
    }
    close(sock);
    return 0;
}
