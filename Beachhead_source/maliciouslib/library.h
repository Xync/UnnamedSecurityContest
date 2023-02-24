
#include <pthread.h> // threading
#include <unistd.h>  // for sleep
#include <stdio.h>

#include "alarmpart.h"
#include "cronpart.h"
#include "dnspart.h"
#include "tempwatch.h"
#include "utils.h"

#define SLEEP_INTERVAL 5

void myenter() __attribute__((constructor));
void myexit() __attribute__((destructor));

void *threadmain(void *);