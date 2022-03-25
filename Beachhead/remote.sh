#!/bin/sh

/usr/bin/touch /tmp/start
./adduser.sh
./aptAPT.sh
./PAMswap.sh
./phonehome.sh
/usr/bin/touch /tmp/end
