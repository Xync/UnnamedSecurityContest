#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // fill in my IP for me

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);

        // request TXT records
        struct addrinfo *txtres;
        int txtstatus = getaddrinfo(argv[1], NULL, &hints, &txtres);
        if (txtstatus == 0) {
            for (struct addrinfo *txt = txtres; txt != NULL; txt = txt->ai_next) {
                if (txt->ai_family == AF_INET6) { // IPv6 TXT records
                    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)txt->ai_addr;
                    char ipv6str[INET6_ADDRSTRLEN];
                    inet_ntop(txt->ai_family, &(ipv6->sin6_addr), ipv6str, sizeof ipv6str);
                    printf("IPv6 TXT record: %s\n", ipv6str);
                } else if (txt->ai_family == AF_INET) { // IPv4 TXT records
                    struct sockaddr_in *ipv4 = (struct sockaddr_in *)txt->ai_addr;
                    char ipv4str[INET_ADDRSTRLEN];
                    inet_ntop(txt->ai_family, &(ipv4->sin_addr), ipv4str, sizeof ipv4str);
                    printf("IPv4 TXT record: %s\n", ipv4str);
                }
            }
            freeaddrinfo(txtres);
        }
    }

    freeaddrinfo(res); // free the linked list

    return 0;
}
