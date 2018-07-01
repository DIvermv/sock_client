
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
	 for(int t=0;t<100;t++)
	 {
    if((send(sock, message, strlen(message), 0))!=sizeof(message))//отправляем сообщение
	    perror("send");
    printf("send:%s size: %ld number %i\n",message,strlen(message), t);
    //sleep(1);
    recv(sock, buf, strlen(message), 0); // получаем сообщение
    
    printf("receive: %s\n",buf);
	 }
    close(sock);
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

    printf(" send %s\n", message); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    for( int i=0;i<20;i++)
    {
    sendto(sock, message, strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr));
    close(sock);

      while(1)
     {
         bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
         buf[bytes_read] = '\0';
         printf("receive:%i  %s\n",bytes_read,buf);
         if(bytes_read>0)
		 break;
     }
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
