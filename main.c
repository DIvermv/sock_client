
#include "sock_client.h"

int main (int argc, char *argv[])
{
// TCP_client(3428, "Test message");
 UDP_client(3429, "Test message");
//   UDP_con_client(3426, "Test message");
printf("!parent end!");
 sleep(1000);    
	return 0;
}
