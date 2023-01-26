#!/bin/bash -x

# Add a new user (with sudo privs) and set a password for root
useradd -m johndoe
echo 'johndoe:RedTeam123' | chpasswd
if [ -f "/etc/debian_version" ]; then
    usermod -aG sudo johndoe
else
    usermod -aG wheel johndoe
fi
echo 'root:Secure123' | chpasswd
# Allow root ssh login
sed -i 's/^#PermitRootLogin .*$/PermitRootLogin yes/' /etc/ssh/sshd_config
# Fedora allows password auth by default, CentOS doesn't
sed -i 's/^PasswordAuthentication .*$/PasswordAuthentication yes/' /etc/ssh/sshd_config

systemctl restart sshd