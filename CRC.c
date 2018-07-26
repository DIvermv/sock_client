#include "sock_client.h"
    u_short checksum(u_short *buffer, int size)
    {
       unsigned long cksum=0;

       while(size > 1) { cksum+=*buffer++; size -= 2; }
       if(size) cksum += *(unsigned char*)buffer;

       cksum = (cksum >> 16) + (cksum & 0xffff);
       cksum += (cksum >>16);
       return (u_short)(~cksum);
    }
