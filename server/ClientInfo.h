#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <time.h>
#include <vector>
#include <string>
#include <map>

class ClientInfo
{
private:
   std::string ip;
   std::string listenPort;
   time_t connectTime;
   std::vector <std::string> share;
public:
    ClientInfo(std::string IP, std::string port);
    ClientInfo();
    std::string getInfo(std::string name);
    std::string getIPPort();
};

typedef std::map <std::string, ClientInfo> Clients;

#endif // CLIENTINFO_H