#include "ClientInfo.h"
#include <sstream>

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
   double elapsedTime = difftime(time(NULL), connectTime);
   os << elapsedTime;
   
   return info + ": port: " + listenPort + " time elapsed: " + os.str() + " seconds";
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
      return "No shared files";
   
   std::string shareList = std::string(share[0]);
   for (unsigned int i = 1; i < share.size(); i++) {
      shareList.append(", ").append(share[i]);
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

ClientInfo* get_client_by_sock(Clients& clients, int sockfd)
{
   /* cauta clientul dupa socket */
   std::map <std::string, ClientInfo> ::iterator it;
   
   for (it = clients.begin(); it != clients.end(); it++)
      if ((*it).second.getSock() == sockfd)
	 return &(*it).second;
      
   ClientInfo a(-100, "not_found", "not_found");
   return &a;
}

bool ClientInfo::hasFile(std::string file)
{
   /* Verifica daca fisierul este la share */
   
   for (unsigned int i = 0; i < share.size(); i++) {
      if (share[i] == file)
	 return true;
   }
   
   return false;
}