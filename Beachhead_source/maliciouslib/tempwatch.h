#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#ifdef LINUX_VERSION
#include <sys/socket.h> // socket stuff
#include <arpa/inet.h>  // inet_addr
#include <netdb.h>
#include <unistd.h>     // for sleep
#endif

int check_temp_tasks();
extern char* ME;

#define _snprintf snprintf