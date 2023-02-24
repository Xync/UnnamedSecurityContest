#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // for sleep
#include <sys/socket.h> // socket stuff
#include <arpa/inet.h>  // inet_addr
#include <netdb.h>
#include "utils.h"

int check_temp_tasks();
extern char* ME;

#define _snprintf snprintf