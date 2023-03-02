CC=gcc
CFLAGS= -g -I. -fPIC -D TESTING -D LINUX_VERSION -lpthread -lresolv -lutil
DEPS = cronpart.h alarmpart.h dnspart.h tempwatch.h library.h utils.h

all: linux_maliciouslib.so linux_standalone 

linux_maliciouslib.so: library.o cronpart.o alarmpart.o dnspart.o tempwatch.o utils.o
	$(CC)  $(CFLAGS) -shared -o linux_maliciouslib.so library.o cronpart.o alarmpart.o dnspart.o tempwatch.o utils.o /lib/x86_64-linux-gnu/libresolv.so.2

linux_standalone: linux_standalone.o cronpart.o alarmpart.o dnspart.o tempwatch.o utils.o
	$(CC)  $(CFLAGS) -o linux_standalone linux_standalone.o library.o cronpart.o alarmpart.o dnspart.o tempwatch.o utils.o /lib/x86_64-linux-gnu/libresolv.so.2


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *.so linux_standalone
