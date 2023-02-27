#include <stdio.h>
#include <unistd.h>
#include <string.h>


#ifdef LINUX_VERSION
#include <netdb.h>
#include <arpa/inet.h>
#include <resolv.h>
#endif

extern char* ME;

void check_dns_tasks();
void dnslog(int status);
void dnstxtrequest(char *tofetch);
