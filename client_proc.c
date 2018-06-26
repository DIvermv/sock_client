
#include "sock_client.h"
int TCP_client(int port, char* message)
{

   // char message[] = "Test message\n";
    char buf[sizeof(message)];

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);// IP4 TCP сокет
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

    send(sock, message, sizeof(message), 0);//отправляем сообщение
    recv(sock, buf, sizeof(message), 0); // получаем сообщение
    
    printf("%s\n",buf);
    close(sock);
    return 0;
}
int UDP_client(int port, char *message)
{

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sendto(sock, message, sizeof(message), 0, (struct sockaddr *)&addr, sizeof(addr));

    close(sock);
    return 0;
}
