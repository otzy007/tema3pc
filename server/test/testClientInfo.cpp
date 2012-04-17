#include <iostream>
#include "../ClientInfo.h"
#include <string>
#include <map>
#include <unistd.h>

using namespace::std;
int main(int argc, char **argv) {
   Clients clients;
   clients["gogu"] = ClientInfo("10.0.0.1", "4424");
   if (argc > 1) {
      ClientInfo c(argv[2], argv[3]);
      clients[argv[1]] = c;
   }
   sleep(2);
   cout << clients["gogu"].getInfo("gogu") << endl;
   cout << clients[argv[1]].getInfo(argv[1]) << endl;
   return 0;
}