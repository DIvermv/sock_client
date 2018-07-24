
#include "sock_client.h"

int main (int argc, char *argv[])
{
Raw_client(3433, "Test message");
printf("!parent end!\n");
// sleep(1000);    
	return 0;
}
