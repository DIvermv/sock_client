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
#endif
