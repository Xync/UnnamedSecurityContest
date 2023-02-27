#include <stdbool.h>  // bool type
#include <sys/stat.h> // stat
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX_VERSION
#include <unistd.h>
#endif

#ifdef WINDOWS_VERSION
#include <windows.h>
#endif


bool file_exists(char *filename);
char* getmyhostname();
void setmyhostname();
void runtask(char *task);
void makepath(char* dest, int destlen, char* front, char* back);
int get_content_length(char *response);
void webrequest(char* ip, int port, char* path, char* outpath);

void base64encode(char** in, char** out);

void base64decode(char** in, char** out);