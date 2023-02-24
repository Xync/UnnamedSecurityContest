#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <resolv.h>

extern char* ME;

void check_dns_tasks();
void dnslog(int status);
void dnstxtrequest(char *tofetch);
