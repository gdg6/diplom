#define  _XOPEN_SOURCE 700
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

class  Sock_t
{
public: 
  struct addrinfo hint, *rez;
  int sock, fd;
  int status;
};


class Server {
private: 
  Sock_t sock_t;
public: 
  Server (char * ip, char * port)
  {
     memset(&sock.hint, 0, sizeof(struct sock_t));
     sock.hint.ai_family = AF_INET;
	 sock.hint.ai_socktype = SOCK_STREAM;
	 sock.status = 0;
	 if((sock.status = getaddrinfo(argv[1], argv[2], &(sock.hint),&(sock.rez)))!=0){
     printf("%s",gai_strerror(sock.status));freeaddrinfo(sock.rez);return BUG_GET;}
     sock.sock = socket(sock.rez->ai_family,sock.rez->ai_socktype,sock.rez->ai_protocol);
	
  	 if(sock.sock == -1){FreeStruct(sock, BUG_SOCK);}
     if(bind(sock.sock, sock.rez->ai_addr, sock.rez->ai_addrlen)!=0){FreeStruct(sock,BUG_BIND);}
     if(listen(sock.sock, 7)!=0){FreeStruct(sock, BUG_LISTEN);}
  }

};


int main()
{
   Server
   return 0;
}
