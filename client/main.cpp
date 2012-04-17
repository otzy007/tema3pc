#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 256

using namespace::std;
int main(int argc, char **argv) {
   if (argc < 4) {
      cout << "Usage: " << argv[0] << "client_name server_ip " <<
       "server_port" << endl; 
      return 0;
   }
   
   int sockfd, fdmax;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   fd_set read_fds;
   fd_set tmp_fds;
   
   char buffer[BUFLEN];
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0)
      cout << "ERROR opening socket\n";
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[3]));
   inet_aton(argv[2], & serv_addr.sin_addr);
  
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      cout << "ERROR connecting\n";
   
   /* trimite numele de utilizator */
   bzero(buffer, BUFLEN);
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
   } else if (buffer[0] == 200) {
      /* Totul e OK */
      
   }
   
   return 0;
}
