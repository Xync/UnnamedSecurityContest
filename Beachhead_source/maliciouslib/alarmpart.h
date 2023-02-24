#include <stdio.h>
#include <time.h>
#include "utils.h"


typedef struct taskstruct {
    int day;
    int hour;
    int min;
    char *task;
    int done;
} taskstruct;

#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6



void check_time_tasks();
