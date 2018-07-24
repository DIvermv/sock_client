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
    u_short recv_length;

    sock = socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
   // addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("192.168.0.8");
   // addr.sin_addr.s_addr = inet_addr("8.8.8.8");
    UDP_Header.dest_port = htons(port);// записываем номер порта получателя
    UDP_Header.src_port = htons(7777);// записываем номер порта отправителя - не принципиален
    for(int i=0 ;i<20;i++)
    {
	   strcpy(buf,"");
	   strcpy(mes,"");

	    sprintf(mes,"send message %i",i);// сформировали сообщение
           UDP_Header.length=htons(sizeof(UDP_Header)+strlen(mes));// посчитали размер нагрузки
	   memcpy((void *)buf,(void *)&UDP_Header,sizeof(UDP_Header));
           memcpy((void *)(buf+sizeof(UDP_Header)),(void *)mes,strlen(mes));
	    sendto(sock, buf, sizeof(UDP_Header)+strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));
	    sleep(1);
         bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
	 /*получили пакет, нужно проверять его правильность*/
		/*	 for(int i=0;i<28;i++)
				 printf("%i=%X ",i,(unsigned char)buf[i]);
				 printf("\n ");*/
	 if(buf[0]==0x45) // наверное IP
	     if(buf[9]==0x11)// наверное UDP
		 if((buf[22]==0x1E) && (buf[23]==0x61))// порт 7777
		 {
                   //perror("send");
      	           //buf[bytes_read] = '\0';
                   memcpy(&recv_length,buf+24,2);
	           recv_length=htons(recv_length)-8;// получаем размер данных из поля UDP
	           memcpy(mes,buf+28,recv_length);// получен пакет вместе с IP-заголовком, поэтому надо отрезать IP - 20 байт и UDP - 8 байт
                   printf("receive:%i bytes: %s\n",recv_length,mes);
		 }
    }
    close(sock);
    return 0;
}
