#include <stdio.h>
#include <string.h>


#ifdef LINUX_VERSION
#include <netdb.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <unistd.h>
#endif

#ifdef WINDOWS_VERSION
#include <Windows.h>
#include <windns.h>
#pragma comment(lib, "Dnsapi.lib") // link against the Dnsapi library
#endif


extern char* ME;

void check_dns_tasks();
void dnslog(int status);
void dnstxtrequest(char *tofetch);
