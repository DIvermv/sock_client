#include "sock_client.h"

int Raw_client(int port, char *message)
{

    int sock,sock_e;
    struct sockaddr_in addr;
    struct UdpHead UDP_Header;
    struct IPv4Head IP_Header;
    struct EthHead EthHeader;
    struct sockaddr_ll addr_ll;
    char buf[1024],buf_e[1560];
    char mes[1016];
    int bytes_read;
    socklen_t addrlen;
    u_short recv_length;

  //  sock = socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
   // sock = socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    sock_e = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
       unsigned char u[6]={0x08,0x00,0x27,0xC6,0xA4,0xFA};
       memcpy(EthHeader.dest,u,6);
       // EthHeader.dest={0x28,0x3B,0x82,0x6D,0x73,0xAF};
       unsigned char u1[6]={0x2C,0x6F,0xC9,0x09,0x9E,0x9D}; 
       memcpy(EthHeader.sour,u1,6);
       // EthHeader.sour={0x2C,0x6F,0x9C,0x09,0x9E,0x9D};
        EthHeader.next_prot=htons(0x0800);

   IP_Header.Ver=0x45;
   IP_Header.Service=0;
   IP_Header.len=0;
   IP_Header.Id=0;
   IP_Header.offset=64;
   IP_Header.TTL=64;
   IP_Header.Next_Prot=17;// UDP
   IP_Header.CRC=0;
   IP_Header.sours_IP=inet_addr("192.168.0.8");
   IP_Header.dest_IP=inet_addr("192.168.0.36");
   
     


    UDP_Header.dest_port = htons(port);// записываем номер порта получателя
    UDP_Header.src_port = htons(7777);// записываем номер порта отправителя - не принципиален
    for(int i=0 ;i<20;i++)
    {
	   strcpy(buf,"");
	   strcpy(mes,"");
	   addr_ll.sll_family = AF_PACKET;    // Тип сокета
           addr_ll.sll_protocol = htons(ETH_P_ALL);   //Тип применяемого протокола
	   addr_ll.sll_ifindex = 3; //индекс сетевого интерфейса
	    sprintf(mes,"send message %i",i);// сформировали сообщение
           UDP_Header.length=htons(sizeof(UDP_Header)+strlen(mes));// посчитали размер нагрузки
	   IP_Header.len=htons(sizeof(UDP_Header)+strlen(mes)+sizeof(IP_Header));// размер для IP
           
           IP_Header.CRC=checksum(&IP_Header,sizeof(IP_Header));// неправильно посчитана



	   memcpy(buf,&IP_Header,sizeof(IP_Header));
	   memcpy((void *)(buf+sizeof(IP_Header)),(void *)&UDP_Header,sizeof(UDP_Header));
           memcpy((void *)(buf+sizeof(IP_Header)+sizeof(UDP_Header)),(void *)mes,strlen(mes));
           
           memcpy(buf_e,&EthHeader,sizeof(EthHeader));
	   memcpy(buf_e+sizeof(EthHeader),buf,sizeof(IP_Header)+sizeof(UDP_Header)+strlen(mes));
	   // sendto(sock, buf, sizeof(IP_Header)+sizeof(UDP_Header)+strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));

	    sendto(sock_e, buf_e,sizeof(EthHeader)+ sizeof(IP_Header)+sizeof(UDP_Header)+strlen(mes), 0,(struct sockaddr *) &addr_ll, sizeof(addr_ll));
	    //perror("send");
         /* что отправили:*/
	 for(int i=0;i<40;i++)
                                 printf("%X ",(unsigned char)buf_e[i]);
                                 printf("\n ");
	int re=1;
//	while(re)
	{
             addrlen=sizeof(addr);
         bytes_read = recvfrom(sock_e, buf, 1500, 0, (struct sockaddr *) &addr, &addrlen);
	 //perror("read");
	 for(int i=0;i<40;i++)
                                 printf("%X ",(unsigned char)buf_e[i]);
                                 printf("\n ");
	 if(buf[14]==0x45) // наверное IP
	     if(buf[23]==0x11)// наверное UDP
		 if((buf[36]==0x1E) && (buf[37]==0x61))// порт 7777
		 {re=0;
                   //perror("send");
	                 for(int i=0;i<40;i++)
                                 printf("%X ",(unsigned char)buf[i]);
                                 printf("\n ");
      	           //buf[bytes_read] = '\0';
                   memcpy(&recv_length,buf+38,2);
	           recv_length=htons(recv_length)-8;// получаем размер данных из поля UDP
	           memcpy(mes,buf+42,recv_length);// получен пакет вместе с Ethernet(14 байт),  IP-заголовком, поэтому надо отрезать IP - 20 байт и UDP - 8 байт
                   printf("receive:%i bytes: %s\n",recv_length,mes);
		 }
	}
    }
    close(sock);
    return 0;
}
