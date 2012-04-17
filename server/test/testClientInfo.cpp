#include <iostream>
#include "../ClientInfo.h"
#include <string>
#include <map>
#include <unistd.h>

using namespace::std;
int main(int argc, char **argv) {
   Clients clients;
   clients["gogu"] = ClientInfo(1, "10.0.0.1", "4424");
   if (argc > 1) {
      ClientInfo c(2, argv[2], argv[3]);
      clients[argv[1]] = c;
   }
   sleep(2);
   cout << clients["gogu"].getInfo("gogu") << endl;
   cout << clients[argv[1]].getInfo(argv[1]) << endl;
   cout << clients["gogu"].getShare();
   clients[argv[1]].shareFile("haha");
   clients[argv[1]].shareFile("lolz.txt");
   clients[argv[1]].shareFile("cmon.mp3");
   cout << clients[argv[1]].getShare() << endl;
   clients[argv[1]].unshareFile("lolz.txt");
   cout << clients[argv[1]].getShare() << endl;
   clients[argv[1]].unshareFile("haha");
   clients[argv[1]].unshareFile("cmon.mp3");
   cout << clients[argv[1]].getShare() << endl;
//    std::map <std::string, ClientInfo> ::iterator it = get_client_by_sock(clients, 1);
//    cout <<(*it).second.getInfo("merge?");
//    (*it).second.shareFile("haide ma");
//    cout << (*it).second.getShare() << endl;
//    
//    for (it = clients.begin(); it != clients.end(); it++) {
//       cout << (*it).second.getShare() << endl;
//    }
   
//    for (it = clients.begin(); it != clients.end(); it++) {
//       if ((*it).second.getSock() == 1) {
// 	 (*it).second.shareFile("haide");
// 	 break; 
//       }
//    }
//     for (it = clients.begin(); it != clients.end(); it++) {
//       cout << (*it).second.getShare() << endl;
//    }
//    ClientInfo cli = get_client_by_sock(clients, 1);
//    cli.shareFile("haasfgsdhfa");
//    cout << cli.getShare() << endl;
//    cout << clients["gogu"].getShare();
   
   return 0;
}