#!/usr/bin/bash

if [ ! -f "updated.txt" ]; then
    ./apt_update_and_flag.sh
fi

apt install -y rsyslog

#Setup rsyslog to listen on network
echo "module(load=\"imudp\")" >> /etc/rsyslog.conf
echo "input(type=\"imudp\" port=\"514\")" >> /etc/rsyslog.conf
echo "module(load=\"imtcp\")" >> /etc/rsyslog.conf
echo "input(type=\"imtcp\" port=\"514\")" >> /etc/rsyslog.conf
echo ":msg,contains,\"[CCDC\" /root/ccdc_syslog.txt" >> /etc/rsyslog.d/20-ccdc.conf
systemctl restart rsyslog


#should get apache, ftpd, and smbd ready to go for file transfers

