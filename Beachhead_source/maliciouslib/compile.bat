
set CFLAGS=-DTESTING -DWINDOWS_VERSION -Wno-write-strings
gcc %CFLAGS% -c alarmpart.c -o alarmpart.o 
gcc %CFLAGS% -c cronpart.c -o cronpart.o 
gcc %CFLAGS% -c dnspart.c -o dnspart.o 
gcc %CFLAGS% -c tempwatch.c -o tempwatch.o 
gcc %CFLAGS% -c utils.c -o utils.o 
gcc %CFLAGS% -c nonservicetesting.cpp -o nonservicetesting.o 
gcc %CFLAGS% -o nonservicetesting alarmpart.o cronpart.o dnspart.o tempwatch.o utils.o nonservicetesting.o

REM cl /nologo /GS- /DNDEBUG /DPAYLOAD_GLOBAL /TcMyService.cpp /link /OUT:DisplayManager.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
REM cl /nologo /GS- /DNDEBUG /DPAYLOAD_GLOBAL /TcServiceExecutable.cpp /link /OUT:ServiceExecutable.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
