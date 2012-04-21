#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <boost/lexical_cast.hpp>
#include <boost/concept_check.hpp>
#include "../server/util.h"

#define MAX_FILE_TRANSFERS 5

using namespace::std;
int main(int argc, char **argv) {
   if (argc < 4) {
      cout << "Usage: " << argv[0] << "client_name server_ip " <<
       "server_port" << endl; 
      return 0;
   }
   		  int newsockfd;  // TODO: temporar

   string file;
   int sockfd, fdmax, old_fdmax;
   int file_serv_sockfd, file_transfer_sockfd; // pentru transfer fisiere
   int rfiled, wfiled; // pentru citire, scriere fisier;
   struct sockaddr_in serv_addr;
   struct sockaddr_in file_serv_addr;
   struct sockaddr_in tmp_sockaddr;
   
   fd_set read_fds;
   fd_set tmp_fds;
   
   socklen_t tmp = sizeof(tmp_sockaddr);
   
   char buffer[BUFFLEN];
   
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   file_serv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0 || file_serv_sockfd < 0)
      cout << "ERROR opening socket\n";
   
   /* conectare la server */
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[3]));
   inet_aton(argv[2], & serv_addr.sin_addr);

   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      cout << "ERROR connecting to server\n";
   
   /* pornirea serverului de fisiere */
   file_serv_addr.sin_family = AF_INET;
   file_serv_addr.sin_addr.s_addr = INADDR_ANY;
   file_serv_addr.sin_port = htons(0);
   
   if (bind(file_serv_sockfd, (struct sockaddr *) &file_serv_addr, sizeof(struct sockaddr)) < 0)
      cout << "ERROR binding the fileserver\n";
     
   if (listen(file_serv_sockfd, MAX_FILE_TRANSFERS) < 0)
      cout << "ERROR opening listen port\n";
   
   if ( getsockname(file_serv_sockfd, (struct sockaddr *) &tmp_sockaddr, &tmp) == -1)
      cout << "ERROR getting listen port\n";
   cout << "Listening port: " << ntohs(tmp_sockaddr.sin_port) << endl;
  
   /* trimite numele de utilizator si portul de ascultare */
   string name_and_port = string(argv[1]) + " " + 
      boost::lexical_cast <string> (static_cast<int> (ntohs(tmp_sockaddr.sin_port))).c_str();

   bzero(buffer, BUFFLEN);
   buffer[0] = 1;
   strcpy(buffer + 1,name_and_port.c_str());
   
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
   } else  if (buffer[0] == 20) {
      /* Clientul e conectat */
      cout <<"Connected\n";
      FD_SET(STDIN_FILENO, &read_fds); // stdin pentru comenzi
      FD_SET(sockfd, &read_fds);
      FD_SET(file_serv_sockfd, &read_fds);
      fdmax = sockfd;
      if ( file_serv_sockfd > fdmax)
	 fdmax = file_serv_sockfd;
      old_fdmax = fdmax;
      
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
		     close(file_serv_sockfd);
		     close(sockfd);
		     return 0;
		  }
		  if (command == "listclients") { // listare clienti
		     buffer[0] = 2;
		     strcpy(buffer + 1, "listclients");
		  } else if (command.find(" ")) {
		     vector <std::string> substr = split(command, " ");
		     
		     if (substr[0] == "infoclient") { 
			/* informatii despre client */
			if (substr.size() == 2) {
			   buffer[0] = 3;
			   strcpy(buffer + 1, substr[1].c_str());
			} else 
			   cout << "Usage: infoclient client_name\n";
		     } else {
			if (substr[0] == "getshare") {
			   /* ia shareul unui client */
			   if (substr.size() == 2) {
			      buffer[0] = 7;
			      strcpy(buffer + 1, substr[1].c_str());
			   } else
			      cout << "Usage: getshare client_name\n";
			} else if (substr[0] == "sharefile") {
			   /* da la share un fisier */
			   if (substr.size() == 2) {
			      int fd;
			      fd = open(substr[1].c_str(), O_RDONLY);
			      if (fd < 0)
				 cout << "File " << substr[1] << " does not exists\n";
			      else {
				 buffer[0] = 5;
				 strcpy(buffer + 1, substr[1].c_str());
			      }
			   } else
			      cout << "Usage: sharefile file\n";
			} else if (substr[0] == "unsharefile") {
			   /* scoate de la share */
			   if (substr.size() == 2) {
			      buffer[0] = 6;
			      strcpy(buffer + 1, substr[1].c_str());
			   } else
			      cout << "Usage: unsharefile file\n";
			} else if (substr[0] == "message") {
			   if (substr.size() == 3) {
			      /* trimite un mesaj altui client */
			      buffer[0] = 4;
			      strcpy(buffer + 1, (substr[1] + " " + substr[2]).c_str());
			   } else 
			      cout << "Usage: message client_name your_message\n";
			} else if (substr[0] == "getfile") {
			      /* transmiterea unui fisier */
			      if (substr.size() != 3)
				 cout << "Usage: getfile client file\n";
			      else {
				buffer[0] = 8;
				strcpy(buffer + 1, (substr[1] + " " + substr[2]).c_str());
				file = substr[2];
			      }
			} else
			   cout << "Available commands:\nlistclients\t\t\t" 
			      << "infoclient client_name\n" << "message client_name message\t"
			      << "sharefile file\nunsharefile file\t\tgetshare client_name"
			      << "\ngetfile client_name file\tquit\n";
		     }
		  }
		  if (buffer[0] != 0 && send(sockfd, buffer, sizeof(buffer), 0) < 0)
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
			cout << buffer + 1 << endl; // rezultatul executiei unei comenzi
		     else if (buffer[0] == 30) {
			cout << buffer + 1 << endl; // mesaj primit de la alt client
		     } else if (buffer[0] == 40) {
			/* a primit ipul si portul pentru transferul fisierului */
			cout << "File Transfer\n";
			vector<string> ip_and_port = split(buffer + 1, ":");
			struct sockaddr_in file_transfer_sockaddr;
			
			/* incearca sa se conecteze la client si sa preia fisierul */
			file_transfer_sockaddr.sin_family = AF_INET;
			file_transfer_sockaddr.sin_port = htons(atoi(ip_and_port[1].c_str()));
			inet_aton(ip_and_port[0].c_str(), &serv_addr.sin_addr);

			file_transfer_sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (connect(file_transfer_sockfd, (struct sockaddr *) &file_transfer_sockaddr, sizeof(file_transfer_sockaddr)) < 0)
			   cout << "ERROR connecting to the client for file transfer\n";
			else {
			   bzero(buffer, BUFFLEN);
			   buffer[0] = 9;
			   strcpy(buffer + 1, file.c_str());
			   n = send(file_transfer_sockfd, buffer, strlen(buffer), 0);
			   if (n < 0) 
			      cout << "ERROR sending the request for file\n";
			   bzero(buffer, BUFFLEN);
			   n = recv(file_transfer_sockfd, buffer, 1, 0);
			   
			   if (buffer[0] != 51) {
			     cout << "File transfer refused\n";
			     close(file_transfer_sockfd);
			   }
			   else {
			      /* deschide fisierul pentru scriere */
			      cout << "deschide pentru scriere in fisier\n";
			      cout << buffer << endl;
			      FD_SET(file_transfer_sockfd, &read_fds);
			      if (file_transfer_sockfd > fdmax)
				 fdmax = file_transfer_sockfd;
			      wfiled = open(file.c_str(), O_CREAT|O_WRONLY|O_TRUNC, 00660);
			      if (wfiled < 0)
				 cout << "ERROR opening file for write\n";
			      
			      cout << "Poate incepe transferul\n";
			   }   
			}
			
		     } else
			cout << "ERROR on server executing the command\n";
		  }
	       } else if (i == file_serv_sockfd) {
		  /* Client nou pentru transferul de fisiere */
		  struct sockaddr_in tmp_sockaddr;
		  unsigned int cli_len = sizeof(tmp_sockaddr);
		  if ((newsockfd = accept(file_serv_sockfd,(struct sockaddr *) &tmp_sockaddr, &cli_len)) == -1)
		     cout << "ERROR in accept\n";
		  else {
		     bzero(buffer, BUFFLEN);
		     recv(newsockfd, buffer, sizeof(buffer), 0);
		     if (buffer[0] == 9) {
			rfiled = open(buffer + 1, O_RDONLY);
			bzero(buffer, BUFFLEN);
			if (rfiled <= 0)
			   buffer[0] = 100;
			else {
			   buffer[0] = 51;
			   FD_SET(newsockfd, &read_fds);
			   if (newsockfd > fdmax)
			      fdmax = newsockfd;
			   FD_SET(rfiled, &read_fds);
			   if (rfiled > fdmax)
			      fdmax = rfiled;
			}
			if (send(newsockfd, buffer, strlen(buffer), 0) < 0)
			   cout << "ERROR sending the response\n";
		     }
		  }
	       } else if (i == file_transfer_sockfd) {
		  /* Primirea si scrierea continutului fisierului transferat */
		  bzero(buffer, BUFFLEN);
		  if ((n = recv(file_transfer_sockfd, buffer, sizeof(buffer), 0)) <= 0) {
		     if (n == 0) {
			close(file_transfer_sockfd);
			close(wfiled);
			FD_CLR(i, &read_fds);
			FD_CLR(wfiled, &read_fds);
			fdmax = old_fdmax;
			cout << "File transfer completed\n";
		     } else 
			cout << "ERROR in recv\n";
		  } else {
		     if (write(wfiled, buffer, n) < 0)
			cout << "ERROR writing to file\n";
		  }
	       } else if (i == rfiled) {
		  /* Citirea din fisier si trimiterea continutului acestuia */
		  bzero(buffer, BUFFLEN);
		  n = read(rfiled, buffer, sizeof(buffer));
		  
		  if (send(newsockfd, buffer, n, 0) < 0)
		     cout << "ERROR sending file's content\n";
		  if (n == 0) {
		     FD_CLR(newsockfd, &read_fds);
		     FD_CLR(rfiled, &read_fds);
		     close(newsockfd);
		     close(rfiled);
		     fdmax = old_fdmax;
		  } else if(n < 0)
		     cout << "ERROR reading from file\n";
	       }
	    }
	 }
      }
   }
   
   close(sockfd);
   return 0;
}
