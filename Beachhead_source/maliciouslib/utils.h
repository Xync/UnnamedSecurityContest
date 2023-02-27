#include <stdbool.h>  // bool type
#include <sys/stat.h> // stat
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX_VERSION
#include <unistd.h>
#endif

#ifdef WINDOWS_VERSION

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

extern int WSAAPI getaddrinfo (const char*,const char*,const struct addrinfo*,
		        struct addrinfo**);
extern void WSAAPI freeaddrinfo (struct addrinfo*);
#endif


bool file_exists(char *filename);
char* getmyhostname();
void setmyhostname();
void runtask(char *task);
void makepath(char* dest, int destlen, char* front, char* back);
int get_content_length(char *response);
void webrequest(char* ip, int port, char* torequest, char* outpath);

void base64encode(char** in, char** out);

void base64decode(char** in, char** out);