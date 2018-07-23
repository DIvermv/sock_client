#ifndef FUNC_H
#define FUNC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 #include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//int TCP_client(int port, char *message);
int UDP_client(int port, char *message);
//int UDP_con_client(int port, char *message);
#endif
