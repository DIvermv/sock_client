#ifndef FUNC_H
#define FUNC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 #include <unistd.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int Raw_client(int port, char *message);
    struct UdpHead
     {
      u_short src_port;// все поля по 2 байта
      u_short dest_port;
      u_short length;
      u_short checksum;
     };
    struct IPv4Head
     {
	unsigned char Ver;// 1-й байт
	     unsigned char Service;
	     u_short       len;
	     u_short       Id;
	     u_short       offset;
	     unsigned char TTL;
	     unsigned char Next_Prot;
	     u_short       CRC;
	     uint32_t      sours_IP;
	     uint32_t      dest_IP;
     };

#endif
