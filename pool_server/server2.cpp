#define _XOPEN_SOURCE 700
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

//Author Grachev Denis
//Server on Fork.
//Example starting: ./server localhost 3000 path
#define DEBUG 1

#define BUG_ARG 1
#define BUG_GET 2
#define BUG_SOCK 3
#define BUG_BIND 4
#define BUG_LISTEN 5
#define BUG_ACCEPT 6

#define SIZE 256

typedef struct sock_t
{
  struct addrinfo hint, *rez;
  int sock, fd;
  int status;
}sock_t;

typedef struct InfoOfFile{
  char path[20]; //путь к файлу
  int  file; 	 //дискриптор файла
  int err;
  int offset;    // кол-во байт отступа от начала файла по дискриптору.
}InfoOfFile;

typedef struct client_t
{
  pid_t pid;
  int fd;
  int err;
  int buf_size;
  char * buf_r;
  char * buf_w;
}client_t;

char * savebuf_r=NULL;
char * savebuf_w = NULL;

int FreeStruct(sock_t sock, int x)
{
  if(x > BUG_SOCK )close(sock.sock);
  if(x > BUG_GET) freeaddrinfo(sock.rez);
  exit(x);	
}


void MyStrtok(char * str, int sizestr, char ** p,int * argc)
{
  *argc = 0;
  p[0] = strtok(str," ");
  if(!p[0])return;
  for(int i = 1; i < 5; i++)
    {
      (*argc)++;
      p[i] = strtok(NULL, " ");
      if(!p[i])return;	
    }
}

void OPT(char str[], int size,InfoOfFile  *p)
{
  char * argv[5];
  int argc;
  MyStrtok(str, size,argv, &argc);
  if(argc!=3){p->err = 1;return;}
	
  if(strcmp(argv[0], "LOAD"))
    {
      p->err = 2;
      return;
    }
  memset(p->path,0, 20);
  strncpy(p->path,argv[1], strlen(argv[1]));
  p->offset = atoi(argv[2]);
  return;
}

void SaveInfoClient(client_t * cl)
{
  savebuf_r = cl->buf_r;
  savebuf_w = cl->buf_w;
}

void UnitStructClient(client_t * cl, int * fd)
{
  cl->pid = getpid();
  cl->err = 0;
  cl->fd = *fd;
  cl->buf_size = SIZE;
  cl->buf_r = NULL;
  cl->buf_w = NULL;
  cl->buf_r = (char*)malloc(sizeof(char)*SIZE);
  if(cl->buf_r == NULL){fprintf(stderr, "can't malloc memory(r)\n");
														cl->err = 1;}
  cl->buf_w = (char*)malloc(sizeof(char)*SIZE);
  if(cl->buf_r == NULL){fprintf(stderr, "can't malloc memory(w)\n");
													    cl->err = 1;}
}

void CloseClientAndFreeStruct(client_t * cl, int file)
{
  if(file!=-1)close(file);
  if(cl->buf_r!=NULL)free(cl->buf_r);
  if(cl->buf_w!=NULL)free(cl->buf_w);
  if(close(cl->fd)==-1)perror("Close in CloseClientAndFreeStruct");
}

int SendFileToClient(InfoOfFile *file,client_t*client){
  memset(client->buf_w, 0,client->buf_size);
  int len,tmp_len;
  while(1)
    {
      len = read(file->file, client->buf_w, client->buf_size);
      if(len<0)break;
      if(len == 0)break;
      		
      while(len){
	if((tmp_len = write(client->fd, client->buf_w, len))<=0){
	  perror("write");
	  return 1;
	}
	len-=tmp_len;
	client->buf_w+=tmp_len;	
      }
      client->buf_w = savebuf_w;
    }	
  client->buf_w = savebuf_w;
  close(file->file);
  return 0;
}

int type(InfoOfFile file)
{
  struct stat buf;
  if(fstat(file.file, &buf)!=0){
    file.err = 3;
    perror("lstat");
    return file.err;
  }
  if(S_ISREG(buf.st_mode)==1)return 0;
  return 4;
}

void RW(int fd)
{
  client_t cl;
  InfoOfFile file;
  int len;
  UnitStructClient(&cl,&fd);
  SaveInfoClient(&cl);
  if(cl.err){CloseClientAndFreeStruct(&cl,-1);return;}
  while(1){
    file.err = 0;
    errno = 0;
    //read of line command to client and send file
    if((len = read(cl.fd, cl.buf_r, cl.buf_size))<0){
      perror("read of info of file");
      CloseClientAndFreeStruct(&cl,-1);
      break;
    }
    cl.buf_r[len]='\0';
    if(len == 0){
      printf("client %d:disconnect\n", cl.pid);
      CloseClientAndFreeStruct(&cl, -1);
      break;
    }
    OPT(cl.buf_r, strlen(cl.buf_r), &file);
    if(file.err)continue;
    if((file.file = open(file.path, O_RDONLY))==-1){
      perror("open");
      continue;
    }
    if(type(file)){close(file.file);continue;}
    if(lseek(file.file, file.offset, SEEK_SET)==-1){continue;}
    if(SendFileToClient(&file, &cl)){CloseClientAndFreeStruct(&cl,file.file);break;}
  }
   exit(1);
}

int main(int argc, char* argv[])
{
  if(argc != 4)
    {printf("no correct argum!");return 1;}
    if(chdir(argv[3])){perror("work directory");return 1;}
  sock_t sock;
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
  while(1){
		if((sock.fd = accept(sock.sock, sock.rez->ai_addr, &(sock.rez->ai_addrlen)))==-1){
		  FreeStruct(sock, BUG_ACCEPT);
		}
		pid_t pid;
		if((pid = fork()) >= 0)
		  {
		if(pid>0){if(close(sock.fd)==-1)perror("close");}
		else
		  {
			if(close(sock.sock) == -1)perror("close fd");
			RW(sock.fd);
		  }
		  } else{perror("fork");if(close(sock.fd)==-1)perror("close");}
	}
	close(sock.sock);
return 0;	
}

