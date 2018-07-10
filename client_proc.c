
#include "sock_client.h"
int TCP_client(int port, char* message)
{

   // char message[] = "Test message\n";
    char buf[1536];

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);// IP4 TCP сокет
   // sock = socket(AF_INET,  SOCK_DGRAM, 0);// IP4 UDP присоединенный сокет
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;// IP4 сокет
    addr.sin_port = htons(port); // TCP порт
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// замыкаем на себя (127.0.0.1)
   if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) // подсоединяемся к серверу
    {
        perror("connect");
        exit(2);
    }
   int state=0;
   int old_state=-1;
   int f_counter=0;
	 while(state!=5)
	 {
	char  temp[1536];
        strcpy(temp,"inc");	
	if(state==4)
		f_counter++;
	
	if(state!=old_state)
		old_state=state;
	else
		strcpy(temp,"dec");
	if(f_counter==5)
		strcpy(temp,"close");

   int send_bytes=send(sock, temp, strlen(temp), 0);
  // if(send_bytes<=0)
//	  close(sock) ;//отправляем сообщение
   // printf("send:%s size: %ld number %i\n",message,strlen(message), t);
   int read_bytes=recv(sock, buf, 1536, 0); // получаем сообщение
  state=atoi(buf);
    buf[read_bytes] = '\0';
    printf("current state: %s\n",buf);
    sleep(1);
	 }
 //   close(sock);
    return 0;
}
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
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
   connect(sock, (struct sockaddr *)&addr, sizeof(addr)); // подсоединяемся к серверу
        perror("connect");
    for( int i=0;i<20;i++)
    {
        send(sock, message, strlen(message), 0);
        perror("sendto");
        // bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
         bytes_read = recv(sock, buf, 1024, 0);
         perror("recvfrom");
      	 buf[bytes_read] = '\0';
         printf("receive:%i  %s\n",bytes_read,buf);
    }
    close(sock);
    return 0;
}
//-----------------------------------------------------
int UDP_con_client(int port, char *message)
{

    int sock,sock_in;
    struct sockaddr_in addr;
    char buf[1024];
    char* temp;
    char* temp_i;
    int bytes_read;
    socklen_t addrlen;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    printf(" send %s\n", message); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
   if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) // подсоединяемся к серверу
    {
        perror("connect");
        exit(2);
    }

    sock_in = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_in < 0)
    {
        perror("socket_in");
        exit(1);
    }

    printf(" send %s\n", message); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port+1);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    bind(sock_in, (struct sockaddr *)&addr, sizeof(addr));
        perror("bind");
    for( int i=0;i<20;i++)
    {
	sprintf(temp, "%s %i", message, i);
    send(sock, temp, strlen(temp), 0);
//	sleep(1);
         bytes_read = recv(sock_in, buf, 1024, 0);
         buf[bytes_read] = '\0';
         printf("receive:%i bytes.  %s\n",bytes_read,buf);
    }
    close(sock);
    return 0;
}
