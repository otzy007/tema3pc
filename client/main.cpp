#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fcntl.h>

#include <vector>

#define BUFFLEN 256

using namespace::std;
int main(int argc, char **argv) {
   if (argc < 4) {
      cout << "Usage: " << argv[0] << "client_name server_ip " <<
       "server_port" << endl; 
      return 0;
   }
   
   int sockfd, fdmax;
   struct sockaddr_in serv_addr;
   struct sockaddr_in file_serv_addr;
   fd_set read_fds;
   fd_set tmp_fds;
   
   char buffer[BUFFLEN];
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
      cout << "ERROR opening socket\n";
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[3]));
   inet_aton(argv[2], & serv_addr.sin_addr);
  
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      cout << "ERROR connecting\n";
   
   /* trimite numele de utilizator */
   bzero(buffer, BUFFLEN);
   buffer[0] = 1;
   strcpy(buffer + 1, argv[1]);
   int n = send(sockfd, buffer, strlen(buffer), 0);
   
   if (n < 0)
      cout << "ERROR writing to socket\n";
   n = recv(sockfd, buffer, sizeof(buffer), 0);
   
   if (buffer[0] == 100) {
      /* 
       * cand numele exista deja se va afisa mesajul
       * iar programul se va inchide
       */
      cout << "Name: " << argv[1] << " already used. Please change it!\n";
      return 0;
   } else{ if (buffer[0] == 20) {
      /* Totul e OK */
      cout <<"OKKK\n";
      FD_SET(STDIN_FILENO, &read_fds); // stdin pentru comenzi
      FD_SET(sockfd, &read_fds);
      fdmax = sockfd;
      
      while(1) {
	 tmp_fds = read_fds;
	 if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
	    cout << "ERROR in select\n";
	 
	 for (int i = 0; i <= fdmax; i++) {
	    if (FD_ISSET(i, &tmp_fds)) {
	       if (i == STDIN_FILENO) {
		  string command;
		  bzero(buffer, BUFFLEN);
		  getline(cin, command);
		  if (command == "quit") {
		     close(sockfd);
		     return 0;
		  }
		  if (command == "listclients") { // listare clienti
		     buffer[0] = 2;
		     strcpy(buffer + 1, "listclients");
		  } else if (command.find(" ")) {
		     
		     vector <std::string> substr;
		     boost::split(substr, command, boost::is_any_of(" "));
		     
		     if (substr[0] == "infoclient") { // informatii despre client
			buffer[0] = 3;
			strcpy(buffer + 1, substr[1].c_str());
		     } else {
			if (substr[0] == "getshare") {
			   /* ia shareul unui client */
			   buffer[0] = 7;
			   strcpy(buffer + 1, substr[1].c_str());
			} else if (substr[0] == "sharefile") {
			   /* da la share un fisier */
			   int fd;
			   fd = open(substr[1].c_str(), O_RDONLY);
			   if (fd < 0)
			      cout << "File " << substr[1] << " does not exists\n";
			   else {
			      buffer[0] = 5;
			      strcpy(buffer + 1, substr[1].c_str());
			   }
			} else if (substr[0] == "unsharefile") {
			   /* scoate de la share */
			   buffer[0] = 6;
			   strcpy(buffer + 1, substr[1].c_str());
			} else if (substr[0] == "message" && substr.size() == 3) {
			   /* trimite un mesaj altui client */
			   buffer[0] = 4;
			   strcpy(buffer + 1, (substr[1] + " " + substr[2]).c_str());
			}
		     }
		  }
		  if (send(sockfd, buffer, sizeof(buffer), 0) < 0)
			   cout << "Cannot send the command\n";
	       } else if (i == sockfd) {
		  int n;
		  bzero(buffer, BUFFLEN);
		  if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
		     if (n == 0) {
			cout << "Connection with server closed\n";
			close(sockfd);
			return 0;
		     }
		     else
			cout << "ERROR in recv\n";
		  } else {
		     if (buffer[0] == 20)
			cout << buffer + 1<< endl;
		     else if(buffer[0] = 30) {
			cout << buffer + 1 << endl;
		     } else
			cout << "ERROR on server executing the command\n";
		  }
	       }
	    }
	 }
      }
   }}
   close(sockfd);
   return 0;
}
