 #ifndef __SERVER_THREAD_POOL__
#define __SERVER_THREAD_POOL__

#define _XOPEN_SOURCE 700
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <functional>
#include <condition_variable>
#include <sys/types.h>
#include <signal.h>


#include "threadPool.cpp"
#include <arpa/inet.h> 	// for inet_ntop function
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "../serviceImpl/servicePack.h"

#define DEBUG 1
#define COUNT_ATTEMPTS 3

#define SUCCESS 0
#define BUG_SOCK -1
#define BUG_BIND -2
#define BUG_LISTEN -3
#define BUG_ACCEPT -4
#define BUG_GET -5
#define USER_FAIL -6
#define ERR_READ_SOCK -7
#define ERR_BIG_MSG -8

class ThreadPoolServer
{
	std::shared_ptr<ThreadPool> pool;
	UserService & userService;
	struct addrinfo hint, *rez;
    int sock, fd;
	int status;
	int count_connection;
	int max_connect;

	inline void FreeStruct(int x)
	{
	  if(x < BUG_SOCK )close(sock);
	  if(x < BUG_GET) freeaddrinfo(rez);
	}

	int serverStartListen(std::string & ip, std::string & port)
	{
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;
		hint.ai_socktype = SOCK_STREAM;
		hint.ai_flags = 0;

		if((status = getaddrinfo(ip.c_str(), port.c_str(), &hint, &rez)) != 0) {
			freeaddrinfo(rez);
			return BUG_GET;
		}

		if((sock = socket(rez->ai_family, rez->ai_socktype, rez->ai_protocol)) == -1 ) {
			return BUG_SOCK;
		}

		if(bind(sock, rez->ai_addr, rez->ai_addrlen) == -1) {
			return BUG_BIND;
		}

		if(listen(sock, 7) != SUCCESS) {
		    return BUG_LISTEN;
		}

		return SUCCESS;
	}

	static void sendManual(int fd)
	{
		std::fstream fs;
		fs.open("man.txt", std::fstream::in );
		std::string  buf;
		while (!fs.eof())
		{
		  std::getline(fs, buf);
		  buf+='\n';
		  if(write(fd, buf.c_str(), (size_t)buf.length()) < 0)
		  {
			break; // no crash programm
		  }
		}
		fs.close();
	}

	void poweroffSystem()
	{
		kill(0, 9); // SIGKILL self app
	}

	void processMsg(std::string msg, int fd)
	{
		if(msg.find("HELP") == 0) sendManual(fd);
		if(msg.find("POWEROFF") == 0)poweroffSystem();
		return;
	}

	//step by step execute command
	// if user send very big message, is not good user
	int ClientRW(int fd)
	{
		const int SIZE = 10;
		char r_buf[SIZE];
		char * link_buf = r_buf;
		int len;
		int tmp_len;
		std::string msg;
		for(;;)
		{
			link_buf = r_buf;
			len = SIZE;

			memset(r_buf, 0, SIZE);
			if(write(fd, "\n>", 2))
			{
			}
			while((tmp_len = read(fd, link_buf, len - 1)) != 0)
			{
				msg = r_buf;
				if(tmp_len < 0)return ERR_READ_SOCK;
				if((len-=tmp_len) < 0 || tmp_len >= SIZE) return ERR_BIG_MSG;
				if(msg.find("\n") > 0)
				{
					std::cout << msg;
					processMsg(msg, fd);
					break;
				}
				std::cout << tmp_len << std::endl;
				link_buf += tmp_len;
				len -= tmp_len;
			}
		}
		return 0;
	}

	int initClient(int fd)
	{
		char incorrect[]  = "Incorrect login or password\n";
		size_t lenInc = strlen(incorrect);
		std::shared_ptr<User> user;
		for(int i = 0; i < COUNT_ATTEMPTS; i++)
		{
			user = userService.AuthorizationUser(fd);
			if(user -> isActive())
			{
				// call function
				ClientRW(fd);
				break;
			} else
			{
				if(write(fd, incorrect, lenInc) < 0)
				{
					// log error
				}
				continue;
			}
		}
		count_connection--;
		close(fd);
		return SUCCESS;
	}

	int receiving()
	{
		char msgMaxConnect[] = "the server is full, try again later";
		size_t lenMsgMaxConnect = strlen(msgMaxConnect);
		if(status != 0 )
		{
			// logerror
			return status;
		}
		for(;;)
		{
			if((fd = accept(sock, rez->ai_addr, &(rez->ai_addrlen))) == -1){
				//log write
				continue; // no return
			} else {
				if(count_connection < max_connect)
				{
					pool -> runAsync(&ThreadPoolServer::initClient, this, fd);
					count_connection++;
				}
				 else
				{
					if(write(fd, msgMaxConnect, lenMsgMaxConnect) < 0)
					{

					}
					close(fd);
				}
		    }
		}

		return 0;
	}

public:
	ThreadPoolServer(UserService & userService) : userService(userService), status(0), count_connection(0)
	{

	}

	int Run(std::string ip, std::string port)
	{
		count_connection = 0;
		max_connect = sysconf(_SC_NPROCESSORS_ONLN);
		if(max_connect  < 2) max_connect  = 2;
		try{
			pool = std::move(std::shared_ptr<ThreadPool>(new ThreadPool(max_connect)));
		}
		 catch(std::bad_alloc& ba)
		{
			//write log
			pthread_exit(NULL);
		}

		if((status = serverStartListen(ip, port)) != SUCCESS)
		{
			FreeStruct(status);
			//must be write log
			pthread_exit(NULL);
		}
		  //~ exit(x);
		return receiving();
	}

	int getStatus()
	{
		return status;
	}

   ~ThreadPoolServer()
   {
	if(close(sock) == -1)
	{
			//write log
		// std::cout << "close fd" << std::endl;
	}

	// std::cout << "I do it!" << std::endl;
   }

};

void * starterThreadPoolServer(void * db)
{
	UserService userService((sqlite3 *)db);
	ThreadPoolServer threadPoolServer(userService);
	if(threadPoolServer.getStatus() == 0)
	{
		threadPoolServer.Run("localhost", "3001");
	}
	return NULL;
}

#endif

