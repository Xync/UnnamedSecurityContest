#include <stdio.h>
#include <string.h>
#include "utils.h"

#ifdef LINUX_VERSION
#include <netdb.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <unistd.h>
#endif

#ifdef WINDOWS_VERSION
#include <Windows.h>
#include <windns.h>
#endif


extern char ME[100];

void check_dns_tasks();
void dnstxtrequest(char *tofetch, char *out);
