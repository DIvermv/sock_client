#include "sock_client.h"

int Raw_client(int port, char *message)
{

    int sock,sock_in;
    struct sockaddr_in addr;
    struct UdpHead UDP_Header;
    struct IPv4Head IP_Header;
    char buf[1024];
    char mes[1016];
    int bytes_read;
    socklen_t addrlen;
    u_short recv_length;

  //  sock = socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
    sock = socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
//   sock = socket(AF_INET,SOCK_DGRAM,0);
    int v=1;
    setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&v,sizeof(v));

    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
   // addr.sin_family = AF_INET;
   // addr.sin_port = htons(port);
   // addr.sin_addr.s_addr = inet_addr("192.168.0.8");
   // addr.sin_addr.s_addr = inet_addr("8.8.8.8");
   IP_Header.Ver=0x45;
   IP_Header.Service=0;
   IP_Header.len=0;
   IP_Header.Id=0;
   IP_Header.offset=0;
   IP_Header.TTL=64;
   IP_Header.Next_Prot=17;// UDP
   IP_Header.CRC=0;
   IP_Header.sours_IP=inet_addr("192.168.0.8");
   IP_Header.dest_IP=inet_addr("192.168.0.8");



    UDP_Header.dest_port = htons(port);// записываем номер порта получателя
    UDP_Header.src_port = htons(7777);// записываем номер порта отправителя - не принципиален
    for(int i=0 ;i<20;i++)
    {
	   strcpy(buf,"");
	   strcpy(mes,"");

	    sprintf(mes,"send message %i",i);// сформировали сообщение
           UDP_Header.length=htons(sizeof(UDP_Header)+strlen(mes));// посчитали размер нагрузки
	   IP_Header.len=htons(sizeof(UDP_Header)+strlen(mes)+sizeof(IP_Header));// размер для IP
	   memcpy(buf,&IP_Header,sizeof(IP_Header));
	   memcpy((void *)(buf+sizeof(IP_Header)),(void *)&UDP_Header,sizeof(UDP_Header));
           memcpy((void *)(buf+sizeof(IP_Header)+sizeof(UDP_Header)),(void *)mes,strlen(mes));
	    sendto(sock, buf, sizeof(IP_Header)+sizeof(UDP_Header)+strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));
	   // sendto(sock, mes,strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));
	    perror("send");
	     // sleep(1);

	 /*получили пакет, нужно проверять его правильность*/
			/* for(int i=0;i<28;i++)
				 printf("%i=%X ",i,(unsigned char)buf[i]);
				 printf("\n ");*/
	int re=1;
	while(re)
	{
             addrlen=sizeof(addr);
         bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
	 //perror("read");
	 printf("rec pac %X\n",buf[9]);
	 if(buf[0]==0x45) // наверное IP
	     if(buf[9]==0x11)// наверное UDP
		 if((buf[22]==0x1E) && (buf[23]==0x61))// порт 7777
		 {re=0;
                   //perror("send");
      	           //buf[bytes_read] = '\0';
                   memcpy(&recv_length,buf+24,2);
	           recv_length=htons(recv_length)-8;// получаем размер данных из поля UDP
	           memcpy(mes,buf+28,recv_length);// получен пакет вместе с IP-заголовком, поэтому надо отрезать IP - 20 байт и UDP - 8 байт
                   printf("receive:%i bytes: %s\n",recv_length,mes);
		 }
	}
    }
    close(sock);
    return 0;
}
