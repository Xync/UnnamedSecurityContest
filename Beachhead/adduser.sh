#!/bin/bash

# Create a new user named scoring.  Create them as a system user (lower user id)
useradd -m -s /bin/bash scoring
# Set the password to 'StandingBy'
echo 'scoring:StandingBy' | chpasswd
# Put the user in the 'sudo' group on debian or 'wheel' on fedora which will give them sudo permissions
if [ -f "/etc/debian_version" ]; then
    usermod -aG sudo scoring
else
    usermod -aG wheel scoring
fi
#useradd -m -s /bin/bash -p '$6$8ANc9NOl$XBXww7b3Wnamz8bDd0by6TucCkxn7QDImrD/krHNuP7Mg6Hek4YZInzuYy56Nv2dVzCwOwxWns.x9a/uXseaH/' scoring
#echo "${TEAMPASSWD}" | passwd --stdin scoring

mkdir ~scoring/.ssh/
touch ~scoring/.ssh/authorized_keys
chmod go-rwx ~scoring/.ssh/authorized_keys
chown scoring.scoring ~scoring/.ssh/authorized_keys
#echo "${SSHPUBKEY}" >> ~scoring/.ssh/authorized_keysecho "
echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAAAgQDV4HmsuDrNI7MZpsOYxaaOYz5o5ZZ2iGXhK//Mp4tdvLojcBCBX8TZd3o9jOTDhpayVInmvTFDRzNzfz3WxRIvL9jP1s04I/jC4NdD4u1hI1DBP9YaCfYDgd1JZHFueNBWYZpltB/dhQi5es8l+Q0IFCsgWNbQ/oLrx3/cyHugeQ==" >> ~scoring/.ssh/authorized_keys

# Change the timestamp of their home directory to be the last time the kernel was updated (roughly)
touch -r /boot /home/scoring
