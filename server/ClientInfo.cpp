#include "ClientInfo.h"
#include <sstream>

ClientInfo::ClientInfo()
{

}

ClientInfo::ClientInfo(std::string IP, std::string port)
{
   ip = IP;
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



