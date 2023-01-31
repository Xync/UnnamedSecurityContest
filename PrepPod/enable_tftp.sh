#!/usr/bin/bash

apt install atftpd
systemctl start atftpd

# Now you can put stuff in /srv/tftp and it will be served. 
#syslog has xferlog
#can also see with journalctl -u atftpd

# On old windows
# tftp -i 192.160.1.101 GET wget.exe

# TODO:  Figure out this interactive
# on Linux: tftp 192.160.1.101
# GET test.txt
# tftp not installed by default
# on kali it came with tftp-hpa
