
set CFLAGS=-DTESTING -DWINDOWS_VERSION -Wno-write-strings -lws2_32 -ldnsapi -static -static-libgcc -static-libstdc++

gcc -c alarmpart.c -o alarmpart.o %CFLAGS%
gcc -c cronpart.c -o cronpart.o %CFLAGS%
gcc -c dnspart.c -o dnspart.o %CFLAGS%
gcc -c tempwatch.c -o tempwatch.o %CFLAGS%
gcc -c utils.c -o utils.o %CFLAGS%
gcc -c nonservicetesting.c -o nonservicetesting.o %CFLAGS%
g++ -c myservice.cpp -o myservice.o %CFLAGS%
g++ -c ServiceExecutable.cpp -o ServiceExecutable.o %CFLAGS%
gcc -o nonservicetesting tempwatch.o alarmpart.o cronpart.o dnspart.o utils.o nonservicetesting.o   %CFLAGS%
REM g++ -o myservice.exe tempwatch.o alarmpart.o cronpart.o dnspart.o utils.o myservice.o %CFLAGS%
g++ -Wno-write-strings ServiceExecutable.o MyService.o tempwatch.o alarmpart.o cronpart.o dnspart.o utils.o -o serviceExecutable.exe -lstdc++ %CFLAGS%