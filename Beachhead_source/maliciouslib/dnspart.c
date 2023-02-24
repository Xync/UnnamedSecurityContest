#include "dnspart.h"

void check_dns_tasks()
{
    // encode myhostname
    // hit dns name
    // dnstun.multifariousnonsense.com

    // parse response
    // At first will be run tasks in lookup table
    // Eventually want to encode blobs in dns responses
    dnstxtrequest("director.multifariousnonsense.com");
}

void dnslog(int status)
{
}

// Not sure what the return should be?
void olddnstxtrequest(char *tofetch)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* IPv4 */
    hints.ai_socktype = SOCK_DGRAM; /* TCP */
    hints.ai_flags = AI_CANONNAME;  /* Canonical name */

    printf("Attempting to lookup %s\n", tofetch);

    /* Get the DNS records for the given hostname */
    s = getaddrinfo(tofetch, NULL, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return;
    }

    struct addrinfo *txt_rp;
    /* Iterate through the records to find the SOA record */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (txt_rp->ai_canonname != NULL)
        {
            printf("TXT canonical name: %s\n", txt_rp->ai_canonname);
        }

        char canonname[1024];
        if (getnameinfo(rp->ai_addr, rp->ai_addrlen, canonname, sizeof(canonname), NULL, 0, 0) == 0)
        {
            printf("Canonical name: %s\n", canonname);
        }
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
        char ipstr[INET_ADDRSTRLEN];

        if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr)) != NULL)
        {
            printf("IP address: %s\n", ipstr);
        }
        /* Check if this record is an SOA record */
        if (rp->ai_canonname != NULL && strstr(rp->ai_canonname, ".") != NULL &&
            rp->ai_canonname[strlen(rp->ai_canonname) - 1] != '.' && strstr(rp->ai_canonname, "in-addr.arpa") == NULL)
        {
            printf("SOA record: %s\n", rp->ai_canonname);
        }
    }

    freeaddrinfo(result); /* Free the memory allocated by getaddrinfo() */
    return;
}

void dnstxtrequest(char *pszDomain)
{
    unsigned char Buffer[8000] = {0};
    unsigned char Result[2048] = {0};
    const unsigned char *pResult = NULL;
    struct __res_state ResState = {0};
    ns_msg nsMsg = {0};
    ns_rr rr;

    int type = 0;
    int ret = 0;
    int size = 0;
    int len = 0;
    int count = 0;
    int i = 0;
    int res_init = 0;

    ret = res_ninit(&ResState);

    if (ret)
    {
        printf("error initializing resolver\n");
        goto close;
    }
    res_init = 1;

    memset(Buffer, 0, sizeof(Buffer));

    size = res_nquery(&ResState, pszDomain, C_IN, T_TXT, Buffer, sizeof(Buffer) - 1);

    if (0 == size)
        goto close;

    ret = ns_initparse(Buffer, size, &nsMsg);

    if (ret)
        goto close;

    count = ns_msg_count(nsMsg, ns_s_an);

    for (i = 0; i < count; i++)
    {
        ret = ns_parserr(&nsMsg, ns_s_an, i, &rr);

        if (ret)
            goto close;

        type = ns_rr_type(rr);
        if (ns_t_txt == type)
        {
            len = ns_rr_rdlen(rr);
            pResult = ns_rr_rdata(rr);

            if ((len > 1) && (len < sizeof(Result)))
            {
                len--;
                memcpy(Result, pResult + 1, len);
                Result[len] = '\0';
                printf("#%d [%s]\n", i, Result);
            }
        }

    } /* for */

close:

    if (res_init)
        res_nclose(&ResState);

    return;
    //return ret;
}

// I think this should just send packet and not worry about result.
void dnsrequestnowait(char **tofetch)
{
}

/*$hostname = "example.com" $dnsRecords = Resolve - DnsName $hostname - Type SOA $dnsRecords | Format - Table - AutoSize - Property Name, Type, SOA*/
