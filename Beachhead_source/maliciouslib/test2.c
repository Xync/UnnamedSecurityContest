#include <resolv.h>
#include <string.h>

int GetTxtRecord(char *pszDomain)
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

  for (i=0; i<count; i++)
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
    res_nclose (&ResState);

  return ret;
}

int main() {
    GetTxtRecord("director.multifariousnonsense.com");
}