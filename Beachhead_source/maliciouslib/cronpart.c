#include "cronpart.h"

cronstruct cronstructlist[] = {
   // {2, "/usr/bin/xeyes &", 0},
    {5, "/usr/bin/touch /tmp/cronran_5", 0},
    {10, "/usr/bin/touch /tmp/cronran_10", 0}};

void add_cron(int period, char *task)
{
    // Not going to write this yet, but perhaps someday.
}

void check_cron_tasks(void)
{
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    int cronstructlistlen = sizeof(cronstructlist) / sizeof(cronstruct);
#ifdef TESTING
    printf("Cronlist is %d long\n", cronstructlistlen);
#endif
    int i;
    for (i = 0; i < cronstructlistlen; i++)
    {
        time_t current_time = time(NULL);
        // Check now vs lastrun times
        if (difftime(current_time, cronstructlist[i].lastrun) > cronstructlist[i].period * 60)
        {

#ifdef TESTING
            printf("Attempting to run :%s:\n", cronstructlist[i].task);
#endif
            runtask(cronstructlist[i].task);

            // Update the last run time.
            cronstructlist[i].lastrun = current_time;
        }
    }
}
