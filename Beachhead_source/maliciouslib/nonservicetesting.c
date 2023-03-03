#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "utils.h"
#include "dnspart.h"
#include "tempwatch.h"
#include "cronpart.h"
#include "alarmpart.h"

// To compile:
//  gcc nonservicetesting.cpp -o nonservicetesting.exe -Wno-write-strings -lws2_32

#define TIMER_VALUE 5000

int main(int argc, char **argv)
{

    setmyhostname();
    printf ("ME: %s\n", ME);
    while (true)
    {
        check_temp_tasks();
        printf ("After temp tasks\n");
        check_time_tasks();
        check_cron_tasks();
        //check_dns_tasks();
        printf ("After dns tasks\n");
        Sleep(TIMER_VALUE);
    }
}
