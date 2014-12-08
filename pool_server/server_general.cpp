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
#include <memory>
#include <mutex>
#include <set>
#include <iostream>
#include <sys/epoll.h>
#include "libev/ev.h"

#define DEBUG 1

#define BUG_ARG 1
#define BUG_GET 2
#define BUG_SOCK 3
#define BUG_BIND 4
#define BUG_LISTEN 5
#define BUG_ACCEPT 6
#define BUG_FCNTL 7

#define SIZE 256

class  Sock_t
{
public: 
  struct addrinfo hint, *rez;
  int sock, fd;
  int status;
  int epollfd; //description fot epoll
  
  Sock_t()
  {
	rez = NULL;
	memset(&hint, 0, sizeof(addrinfo));
  }
  
  ~Sock_t()
  {
  	if(sock > 0)close(sock);
    if(rez != NULL) freeaddrinfo(rez);
  }

};

class Client {
public: 
  int fd;
  int err;
  int buf_size;
  char buf_r[255]; // размер для считывания комманд от пользователя
  char buf_w[1024];//размер буффера для ответа пользователю 
};

class Server {
private: 

  Sock_t sock;
  std::set<int> client_fds;

  int FreeStruct(Sock_t sock, int x)
  {
	if(x == 0) std::cout << "Succes: 0\n";
	else std::cout << "error: " << x << std::endl;;
    if(x > BUG_SOCK )close(sock.sock);
    if(x > BUG_GET) freeaddrinfo(sock.rez);
    return x;
  }
  
  static int make_socket_non_blocking (int sfd)
 {
	  int flags, s;

	  flags = fcntl (sfd, F_GETFL, 0);
	  if (flags == -1)
	  {
		  perror ("fcntl");
		  return -1;
	  }

	  flags |= O_NONBLOCK;
	  s = fcntl (sfd, F_SETFL, flags);
	  if (s == -1)
	  {
	    perror ("fcntl");
		return -1;
	  }
  return 0;
}
  
  int ProcessingClients()
  {
	struct epoll_event ev, events[10];
	if((sock.epollfd = epoll_create(10)) == -1)
	{
		FreeStruct(sock, errno);
		sock.status = errno;
		return errno;
	}
	
  }
  
public: 
  Server (const char * ip,const char * port)
  {
     sock.hint.ai_family = AF_INET;
	 sock.hint.ai_socktype = SOCK_STREAM;
	 sock.status = 0;
	 if((sock.status = getaddrinfo(ip, port, &(sock.hint),&(sock.rez)))!=0){
     printf("%s",gai_strerror(sock.status));freeaddrinfo(sock.rez);}    
  }
  
  int Run()
  {
	 sock.sock = socket(sock.rez->ai_family,sock.rez->ai_socktype,sock.rez->ai_protocol);
	
  	 if(sock.sock == -1){FreeStruct(sock, BUG_SOCK); return BUG_SOCK;}
     if(bind(sock.sock, sock.rez->ai_addr, sock.rez->ai_addrlen)!=0){FreeStruct(sock,BUG_BIND);return BUG_BIND;}
     if(make_socket_non_blocking(sock.sock) != 0){FreeStruct(sock, BUG_FCNTL); return BUG_FCNTL;} ;
     if(listen(sock.sock, 7)!=0){FreeStruct(sock, BUG_LISTEN); return BUG_LISTEN;}    
     return ProcessingClients();
  }
  
  ~Server()
  {
  }
};


int main()
{
   const char * ip = "localhost";
   const char * port = "5555";
   std::shared_ptr<Server> server = std::shared_ptr<Server>(new Server(ip, port));
   return 0;
}
