#include <stdbool.h>  // bool type
#include <sys/stat.h> // stat
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


bool file_exists(char *filename);
char* getmyhostname();
void setmyhostname();
void runtask(char *task);
int get_content_length(char *response);
void base64encode(char** in, char** out);

void base64decode(char** in, char** out);