#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include "ClientInfo.h"

#define MAX_CLIENTS 5
#define BUFFLEN 256

using namespace::std;
int main(int argc, char **argv) {
   if (argc < 2) {
      cout << "Usage "<< argv[0] <<" port";
      return 0;
   }
   
   // map folosit pentru stocarea informatiilor despre clienti
   Clients clients;
   
   int port = atoi(argv[1]);
   char buffer[BUFFLEN];
   int sockfd;
   int fdmax;
   
   struct sockaddr_in serv_addr;
   map <int, struct sockaddr_in> cli_addr;
   int cli_no; // numarul de clienti
   
   fd_set read_fds;
   
   FD_ZERO(&read_fds);
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
      cout << "ERROR opening socket\n";
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port);
   
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0)
      cout << "ERROR on binding\n";
   
   listen(sockfd, MAX_CLIENTS);
   FD_SET(STDIN_FILENO, &read_fds); // stdin pentru comenzi
   FD_SET(sockfd, &read_fds);
   fdmax = sockfd;
   
   while(1) {
      fd_set tmp_fds = read_fds;

      if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
	 cout << "ERROR in select\n";
      string command;
//       cin >> command;
      for (int i = 0; i <= fdmax; i++) {
	 if (FD_ISSET(i, &tmp_fds)) {
	    if (i == sockfd) {
	       // adauga datele clientului la server
	       int newsockfd;
	       struct sockaddr_in tmp_sockaddr;
	       unsigned int cli_len = sizeof(tmp_sockaddr);
	       if ((newsockfd = accept(sockfd,(struct sockaddr *) &tmp_sockaddr, &cli_len)) == -1)
		  cout << "ERROR in accept\n";
	       else {
		  FD_SET(newsockfd, &read_fds);
		  if (newsockfd > fdmax)
		     fdmax = newsockfd;
		  cli_addr[newsockfd] = tmp_sockaddr;
	       }
	    } else {
	       if (i == STDIN_FILENO) {
		  cin >> command;
		  if (command == "quit")
		     return 0; // opreste serverul la primirea comenzii quit
		  if (command == "status") {
		     if (clients.empty())
			cout << "No clients\n";
		     else {
			map <std::string, ClientInfo> :: iterator it;
			cout << "Client\tIP:Port\t\tShared files\n";
			for (it = clients.begin(); it != clients.end(); it++) {
			   cout << (*it).first << "\t" << (*it).second.getIPPort() <<
			      "\t" << (*it).second.getShare()/* << endl*/;
			}
		     }
		  } else {
		     cout << "Available commands: status, quit\n";
		  }
	       } else {
		  int n;
		  bzero(buffer, BUFFLEN);
		  if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
		     if (n == 0) 
			cout << "connection closed\n";
		     else
			cout << "ERROR in recv\n";
		     close(i);
		     FD_CLR(i, &read_fds);
		  } else {
		     cout << "message recieved: " << buffer << endl;
		     switch (buffer[0]) {
			case 1: 
			{
			   if (clients.find(buffer + 1) != clients.end()) {
			      bzero(buffer, BUFFLEN);
			      buffer[0] = 100;
			      send(i, buffer, strlen(buffer), 0);
			   } else {
			      clients[buffer + 1] = ClientInfo(inet_ntoa(cli_addr[i].sin_addr), "22");
			      buffer[0] = 200;
			      cout << clients[buffer + 1].getIPPort();
			      send(i, buffer, strlen(buffer), 0);
			   }
			} break;
		     }
		  }
	       }
	    }
	 }
      }
      
   }
   close(sockfd);
   return 0;
}
