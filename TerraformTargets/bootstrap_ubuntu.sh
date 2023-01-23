#!/bin/bash -x

# Add a new user (with sudo privs) and set a password for root
useradd -m johndoe
echo 'johndoe:SecretPassword' | chpasswd
usermod -aG sudo johndoe
echo 'root:1NewPassword' | chpasswd
# Allow root ssh login
sed -i 's/^.*PermitRootLogin .*$/PermitRootLogin yes/' /etc/ssh/sshd_config
sed -i 's/^PasswordAuthentication .*$/PasswordAuthentication yes/' /etc/ssh/sshd_config

systemctl restart sshd