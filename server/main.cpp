#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace::std;
int main(int argc, char **argv) {
   if (argc < 2) {
      cout << "Usage "<< argv[0] <<" port";
   }
   int port = atoi(argv[1]);
   return 0;
}
