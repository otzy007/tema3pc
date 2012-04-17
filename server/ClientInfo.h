#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <time.h>
#include <vector>
#include <string>
#include <map>

class ClientInfo
{
private:
   int sockfd;
   std::string ip;
   std::string listenPort;
   time_t connectTime;
   std::vector <std::string> share;
public:
    ClientInfo(int sock, std::string IP, std::string port);
    ClientInfo();
    std::string getInfo(std::string name);
    std::string getIPPort();
    std::string getShare();
    void shareFile(std::string file);
    bool unshareFile(std::string file);
    int getSock();
};

typedef std::map <std::string, ClientInfo> Clients;

ClientInfo* get_client_by_sock(Clients &clients, int sockfd);

#endif // CLIENTINFO_H