#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>

#define SUCCESS 0
#define NOTICE  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

#define SIZE    1024

char* get_date(char* date, int flag);

void print_log(char* str,int level);

int startup(char*ip,char* port);

static int get_line(int sock,char line[],int size);

void echo_string(int scok);

static int echo_www();

static void drop_header(int sock);

static int exe_cgi(int sock,char* method,char* path,char* query_string);

void* handler_request(void* arg);
