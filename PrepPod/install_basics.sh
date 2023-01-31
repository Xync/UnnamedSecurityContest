#!/usr/bin/bash

if [ ! -f "updated.txt" ]; then
    ./apt_update_and_flag.sh
fi

apt install net-tools screen nmap plocate

# armitage triggers lots of download and install.  nvm
