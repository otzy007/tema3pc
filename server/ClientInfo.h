#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <vector>
#include <string>
#include <map>

class ClientInfo
{
private:
   int sockfd; // file descriptorul pe care comunica serverul cu clientul
   std::string ip; // ip-ul clientului
   std::string listenPort; // portul pe care asculta pentru transferul de fisiere
   time_t connectTime; // timpul conectarii
   std::vector <std::string> share; // vector ce contine lista cu fisierele date la share
public:
    ClientInfo(int sock, std::string IP, std::string port);
    ClientInfo();
    std::string getInfo(std::string name); // returneaza nume, port, timp conectare
    std::string getIPPort(); // returneaza IP:port
    std::string getShare(); // returneaza lista cu fisierele de la share
    void shareFile(std::string file); // adauga la lista cu fisiere
    bool unshareFile(std::string file); // elimina din lista
    int getSock(); // returneaza file descriptorul clientului
    bool hasFile(std::string file); // verifica daca clientul are la share fisierul
};

typedef std::map <std::string, ClientInfo> Clients;

ClientInfo* get_client_by_sock(Clients &clients, int sockfd); // cauta clientul in functie de socket

#endif // CLIENTINFO_H