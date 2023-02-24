#include <stdio.h>
#include <time.h>
#include "utils.h"

typedef struct cronstruct {
    int period; // in minutes
    char *task;
    time_t lastrun;
} cronstruct;

void check_cron_tasks();