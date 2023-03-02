#!/bin/bash

sudo /usr/sbin/iptables -F

sudo /usr/sbin/iptables -X

# Add to crontab
sudo /bin/bash -c 'echo "*/5 * * * * root /usr/sbin/iptables -F" >> /etc/crontab'
sudo /bin/bash -c 'echo "*/5 * * * * root /usr/sbin/iptables -X" >> /etc/crontab'