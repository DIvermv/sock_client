
#include "sock_client.h"

int main (int argc, char *argv[])
{
Raw_client(3429, "Test message");
printf("!parent end!");
 sleep(1000);    
	return 0;
}
