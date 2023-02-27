#include <stdio.h>
#include <Windows.h>
#include <sys/stat.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ctype.h> // for isdigit
#include <time.h>
#include "utils.h"
#include "dnspart.h"
#include "tempwatch.h"
#include "cronpart.h"
#include "alarmpart.h"


//To compile:
// gcc nonservicetesting.cpp -o nonservicetesting.exe -Wno-write-strings -lws2_32

#define TIMER_VALUE 5000
#define TESTING 1

FILE *fh;
char thisstr[] = "testing";
char inbuff[1024];

int main(int arc, char **argv) {
    WSADATA version;
    WSAStartup(MAKEWORD(2,2), &version);
    SOCKET sockfd;
    SOCKADDR_IN servaddr;
    char ip[1024];
    char torequest[1024];
    char request[1024];
    char outpath[1024];
    char me[100]; //Some how get the hostname in here
    int port;

    setmyhostname();

    while (true) 
    {
        check_temp_tasks();
        check_time_tasks();
        check_cron_tasks();
        check_dns_tasks();


	    Sleep(TIMER_VALUE);
    }
}
