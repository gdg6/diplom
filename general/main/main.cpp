#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include "App.cpp"

//~ PRAGMA journal_mode=WAL - for async SQLITE
//g++ -std=gnu++11 main.cpp ../utils/bcrypt/bcrypt.a -lnetsnmp -lsqlite3 -lpthread
#define __NO_PRODUCTION__

int main(int argc, char *argv[])
{
#ifndef __NO_PRODUCTION__
	  /* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
           exit(EXIT_FAILURE);
        }
        
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
           exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        
        /* Change the current working directory */
        if ((chdir("./")) < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
#endif

	sqlite3 * db;
	sqlite3_open("../../WebServer/snmp_db", &db);
	App app(db);
	app.Run();
	
	return 0;
}
