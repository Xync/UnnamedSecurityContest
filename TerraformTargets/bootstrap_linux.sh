#cloud-boothook
#!/bin/bash

# The cloud-boothook directive tells AWS to run this script after every reboot.
# This is only needed for the Debian 8 AMI that seems to have been captured after running its own user-data script.
# Since we don't really plan on rebooting these servers, it should be fine to use on all of them.

# Add a new user, special case for Debian 8
useradd -m johndoe
echo 'johndoe:RedTeam123' | chpasswd

# Add user to group authorized for sudo
if [ -f "/etc/debian_version" ]; then
    usermod -aG sudo johndoe
else
    usermod -aG wheel johndoe
fi
# Set a password for root
echo 'root:Secure123' | chpasswd
# Allow root ssh login
sed -i 's/^#PermitRootLogin .*$/PermitRootLogin yes/' /etc/ssh/sshd_config
# Fedora allows password auth by default, CentOS doesn't
sed -i 's/^PasswordAuthentication .*$/PasswordAuthentication yes/' /etc/ssh/sshd_config

# Ubuntu 12 & 14 don't use systemd,
if which systemctl; then
    systemctl restart sshd
else
    service ssh restart
fi
