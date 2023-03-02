#include "alarmpart.h"
#include "cronpart.h"
#include "dnspart.h"
#include "tempwatch.h"
#include "utils.h"

#define SLEEP_INTERVAL 5

int main() {
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