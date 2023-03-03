#include "tempwatch.h"

void check_temp_tasks()
{
  char request[1124];
  char inbuff[1024];
  FILE *fh;
  int tmp;
  char thisstr[] = "this";

  char ip[1024];
  char outpath[1024];
  char torequest[900];
  char path[1024];
  int port;
 
  // Test create file
  makepath(path, 1024, "ABC");
  if (file_exists(path)) // Basic Test
  {
    makepath(path, 1024, "sawabc");
    fh = fopen(path, "w");
    fwrite(thisstr, 1, sizeof(thisstr), fh);
    fclose(fh);

    unlink("c:\\temp\\ABC");
  }

  // Test execute command
  makepath(path, 1024, "ANDOR");
  if (file_exists(path)) // Test Command
  {

#ifdef WINDOWS_VERSION
    char *cmd = "c:\\Windows\\notepad.exe";
#endif
#ifdef LINUX_VERSION
    char *cmd = "touch /tmp/andor_success";
#endif
    runtask(cmd);
    unlink(path);
  }

  // Exec command in file
  makepath(path, 1024, "X9998-lock");
  if (file_exists(path)) // Run Command
  {
    fh = fopen(path, "rb");
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
    fgets(inbuff, sizeof(inbuff), fh);
    fclose(fh);

    // Not sure why but there's a new line coming in that is screwing it up.  fixed it
    if (inbuff[strlen(inbuff) - 1] == '\n')
    {
      inbuff[strlen(inbuff) - 1] = '\0';
    }

#ifdef TESTING
    printf("Trying to start '%s'\n", inbuff);
#endif
    runtask(inbuff);
    unlink(path);
  }

// Download file from port... not doing
#ifdef DEACTIVATED
  makepath(path, 1024, localpath, "X9999-lock");
  if (file_exists(path))
  { // Connect to socket and write to file
    fh = fopen(path, "r");
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
    fgets(inbuff, sizeof(inbuff), fh);
    fclose(fh);
#ifdef TESTING
    printf("About to parse %s\n", inbuff);
#endif

    // Parse inbuff for addr and port separated by space
    if (sscanf(inbuff, "%100s %d %100s", ip, &port, outpath) != 3)
    {
#ifdef TESTING
      printf("Error parsing host and port and path");
#endif
    }
    else
    {
#ifdef TESTING
      printf("ip is: %s\nport is: %d\noutpath is: %s\n", ip, port, outpath);
#endif

      WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

      sockfd = socket(AF_INET, SOCK_STREAM, 0);

      if (sockfd < 0)
      {
#ifdef TESTING
        printf("Error opening socket\n");
#endif
      }

      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(ip);
      servaddr.sin_port = htons(port);

      if (WSAConnect(sockfd, (SOCKADDR *)&servaddr, sizeof(servaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
      {
#ifdef TESTING
        printf("Socket Error\n");
#endif
        closesocket(sockfd);
      }
      else
      {

        unlink(outpath);
        fh = fopen(outpath, "wb");

        int tmp = 1;

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
        }
        fclose(fh);
      }
    }
    unlink("c:\\temp\\X9999-lock");
  }

#endif

  // Web download and write to file
  makepath(path, 1024, "X9997-lock");
  if (file_exists(path))
  {
    fh = fopen(path, "r");
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
    fgets(inbuff, sizeof(inbuff), fh);
    fclose(fh);
#ifdef TESTING
    printf("About to parse :%s:\n", inbuff);
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
      printf("ip is: %s\nport is: %d\noutpath is: %s\n", ip, port, outpath);
#endif

      webrequest(ip, port, torequest, outpath);
    } 
    unlink(path);
  }
}

#ifdef DEACTIVATED
      WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

      //      sockfd = socket(AF_INET, SOCK_STREAM, 0);

      if (sockfd < 0)
      {
#ifdef TESTING
        printf("Error opening socket\n");
#endif
      }

      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(ip);
      servaddr.sin_port = htons(port);

      if (WSAConnect(sockfd, (SOCKADDR *)&servaddr, sizeof(servaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
      {
#ifdef TESTING
        printf("Socket Error\n");
#endif
        closesocket(sockfd);
      }
      else
      {
        // Send request
        _snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", torequest, me);
        send(sockfd, request, sizeof(request), 0);
        Sleep(1000);
        int tmp = recv(sockfd, inbuff, sizeof(inbuff), 0); // probably have the full header portion here.
        // Find the Content-Length
        int toread = get_content_length(inbuff);
        if (toread == -1)
        {
#ifdef TESTING
          printf("didn't find content-length\n");
#endif
        }

        unlink(outpath);
        fh = fopen(outpath, "wb");
        int sofar = 0;

        // Need to process the first portion of the response after the headers.
        // First find the start of the content.
        char *start = strstr(inbuff, "\r\n\r\n");
        if (start == NULL)
        {
#ifdef TESTING
          printf("Couldn't find the boundary between response and body\n");
#endif
        }
        start += 4; // Get past the \r\n\r\n
        int len_of_header = (int)start - (int)inbuff;

        int tmp2 = fwrite(start, sizeof(char), tmp - len_of_header, fh);
#ifdef TESTING
        printf("Wrote %d bytes\n", tmp2);
#endif
        sofar = tmp2;

        while (sofar < toread)
        {
          Sleep(100);
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
        fclose(fh);
      }
    }
#endif
