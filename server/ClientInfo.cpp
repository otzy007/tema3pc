#include "ClientInfo.h"
#include <sstream>
#include <iostream>

ClientInfo::ClientInfo()
{

}

ClientInfo::ClientInfo(int sock, std::string IP, std::string port)
{
   ip = IP;
   sockfd = sock;
   listenPort = port;
   connectTime = time(NULL);
}

std::string ClientInfo::getInfo(std::string name)
{
   std::string info(name);
   std::stringstream os;
//    time_t elapsedTime = time(NULL) - connectTime;
   double elapsedTime = difftime(time(NULL), connectTime);
   os << elapsedTime;
   
   return info + ": port: " + listenPort + " time elapsed: " + os.str();
}

std::string ClientInfo::getIPPort()
{
   std::string info(ip);
   return ip + ":" + listenPort;
}

std::string ClientInfo::getShare()
{
   /* returneaza fisierele de la share */
   if ( share.empty() )
      return "No shared files\n";
   
   std::string shareList = std::string(share[0]);
   for (unsigned int i = 1; i < share.size(); i++) {
      shareList.append(", ").append(share[i]);
//       std::cout << share[i] << std::endl;
   }
   return shareList;
}

void ClientInfo::shareFile(std::string file)
{
   /* adaugare fisier la lista de share */
   share.push_back(file);
}

bool ClientInfo::unshareFile(std::string file)
{
   /* elimina un fisier din lista de share */
   for (unsigned int i = 0; i < share.size(); i++) {
      if (share[i] == file) {
	 share.erase(share.begin() + i);
	 return true;
      }
   }
   return false;
}

int ClientInfo::getSock()
{
   return sockfd;
}





