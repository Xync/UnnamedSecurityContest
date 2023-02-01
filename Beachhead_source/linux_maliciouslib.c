#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h> // threading
#include <unistd.h>  // for sleep
#include <stdbool.h> // bool type
#include <sys/stat.h> // stat

#define TESTING 1

#define SLEEP_INTERVAL 5

// gcc -shared -fPIC -o linux_maliciouslib.so linux_maliciouslib.c
// LD_PRELOAD=./third.so ls

void myenter()__attribute__((constructor));
void myexit()__attribute__((destructor));

void* threadmain(void *);

void myenter() {

#ifdef TESTING
  printf("library loaded\n");
#endif

  pthread_t dontcare;

  pthread_create(&dontcare, NULL, &threadmain, NULL);
}


void myexit() {
//This doesn't seem to be happening, but I'm not entirely sure why.
#ifdef TESTING
  printf("library unloaded\n");
#endif
}

bool file_exists(char* filename) {
  struct stat buffer;
  return (stat (filename, &buffer) == 0);
}

void* threadmain(void *arg) {
  FILE *fh;

  while (1) {
#ifdef TESTING
     printf ("main ran\n");
#endif
     sleep(SLEEP_INTERVAL);
     if (file_exists("/tmp/ABC")) {
       fh = fopen("/tmp/sawabc","w");
       char thisstr[] = "this";
       fwrite(thisstr,1,sizeof(thisstr), fh);
       fclose(fh);
     }
     if (file_exists("/tmp/ANDOR")) { //New User

     }
  }
}
