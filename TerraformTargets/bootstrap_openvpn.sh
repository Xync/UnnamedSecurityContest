#!/bin/bash -x

# Based on https://linuxconfig.org/how-to-setup-a-openvpn-server-on-ubuntu-20-04

# Install Packages
apt-get update
apt-get install -y easy-rsa openvpn

# Setup Certificate Authority
cd /root
make-cadir certificate_authority
cd certificate_authority
mv vars vars.default
echo 'set_var EASYRSA_REQ_COUNTRY    "US"' >> vars
echo 'set_var EASYRSA_REQ_PROVINCE   "CO"' >> vars
echo 'set_var EASYRSA_REQ_CITY       "Denver"' >> vars
echo 'set_var EASYRSA_REQ_ORG        "CCDC"' >> vars
echo 'set_var EASYRSA_REQ_EMAIL      "admin@ccdc.com"' >> vars
echo 'set_var EASYRSA_REQ_OU         "CA"' >> vars
./easyrsa init-pki
echo -en "\n" | ./easyrsa build-ca nopass

# Setup Server Certificates
cd /root
make-cadir server
cd server
mv vars vars.default
echo 'set_var EASYRSA_REQ_COUNTRY    "US"' >> vars
echo 'set_var EASYRSA_REQ_PROVINCE   "CO"' >> vars
echo 'set_var EASYRSA_REQ_CITY       "Denver"' >> vars
echo 'set_var EASYRSA_REQ_ORG        "CCDC"' >> vars
echo 'set_var EASYRSA_REQ_EMAIL      "admin@ccdc.com"' >> vars
echo 'set_var EASYRSA_REQ_OU         "SERVER"' >> vars
./easyrsa init-pki

# Generate a signing request in the server directory and sign it from the CA directory
echo -en "\n" | /root/server/easyrsa gen-req server nopass
cd /root/certificate_authority
./easyrsa import-req /root/server/pki/reqs/server.req server
echo -en "yes\n" | ./easyrsa sign-req server server

# Collect the server key and certificate, and the ca certificate
cp /root/server/pki/private/server.key /etc/openvpn/server
cp /root/certificate_authority/pki/issued/server.crt /etc/openvpn/server
cp /root/certificate_authority/pki/ca.crt /etc/openvpn/server

# Generate DH params and put in place
cd /root/server/
./easyrsa gen-dh
cp pki/dh.pem /etc/openvpn/server/dh2048.pem

# Generate a TA certificate and put in place
openvpn --genkey secret ta.key
cp /root/server/ta.key /etc/openvpn/server

# Copy default server config and update
cp /usr/share/doc/openvpn/examples/sample-config-files/server.conf /etc/openvpn/server/
cd /etc/openvpn/server
#sed -i 's/^cipher AES-256-CBC$/cipher AES-256-GCM\nauth SHA256/' /etc/openvpn/server/server.conf
sed -i 's/^cipher AES-256-CBC$/cipher AES-256-GCM/' /etc/openvpn/server/server.conf
#sed -i 's/^dh dh2048.pem$/dh none/' /etc/openvpn/server/server.conf
sed -i 's/^;user nobody$/user nobody/' /etc/openvpn/server/server.conf
sed -i 's/^;group nobody$/group nogroup/' /etc/openvpn/server/server.conf

# Allow packet forwarding at the kernel level
sed -i 's/^#net.ipv4.ip_forward=1$/net.ipv4.ip_forward=1/' /etc/sysctl.conf
sysctl -p

# Prepend OVPN Routing Rules to the UFW rules file
echo '*nat' >> /tmp/ovpnrules
echo ':POSTROUTING ACCEPT [0:0]' >> /tmp/ovpnrules
echo '-A POSTROUTING -s 10.8.0.0/16 -o eth0 -j MASQUERADE' >> /tmp/ovpnrules
echo 'COMMIT' >> /tmp/ovpnrules
cat /etc/ufw/before.rules >> /tmp/ovpnrules
cp /tmp/ovpnrules /etc/ufw/before.rules
rm /tmp/ovpnrules
#Also modify default forward policy
sed -i 's/^DEFAULT_FORWARD_POLICY="DROP"$/DEFAULT_FORWARD_POLICY="ACCEPT"/' /etc/default/ufw

# Open the OpenVPN port in UFW
ufw allow 1194/udp
ufw allow ssh
ufw disable
ufw -f enable

systemctl enable openvpn-server@server.service
systemctl start openvpn-server@server.service

# Now generate and sign the attacker certificates
cd /root/server
echo -en "\n" | ./easyrsa gen-req attacker nopass
cd /root/certificate_authority
./easyrsa import-req /root/server/pki/reqs/attacker.req attacker
echo -en "yes\n" | ./easyrsa sign-req client attacker
mkdir /home/ubuntu/openvpn-config
cp /root/certificate_authority/pki/issued/attacker.crt /home/ubuntu/openvpn-config/
cp /root/server/pki/private/attacker.key /home/ubuntu/openvpn-config/

# Copy the default config and modify
cp /usr/share/doc/openvpn/examples/sample-config-files/client.conf /home/ubuntu/openvpn-config/base.conf
extip=`curl http://checkip.amazonaws.com`
sed -i "s/my-server-1/$extip/" /home/ubuntu/openvpn-config/base.conf
#sed -i 's/^cipher AES-256-CBC$/cipher AES-256-GCM\nauth SHA256/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^;user nobody$/user nobody/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^;group nobody$/group nogroup/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^ca ca.crt$/;ca ca.crt/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^cert client.crt$/;cert client.crt/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^key client.key$/;key client.key/' /home/ubuntu/openvpn-config/base.conf
sed -i 's/^tls-auth ta.key 1$/;tls-auth ta.key 1/' /home/ubuntu/openvpn-config/base.conf
echo "key-direction 1" >> /home/ubuntu/openvpn-config/base.conf
echo "route 10.23.0.0 255.255.255.0" >> /home/ubuntu/openvpn-config/base.conf
echo "; Use these for non systemd-resolved systems" >> /home/ubuntu/openvpn-config/base.conf
echo "; script-security 2" >> /home/ubuntu/openvpn-config/base.conf
echo "; up /etc/openvpn/update-resolv-conf" >> /home/ubuntu/openvpn-config/base.conf
echo "; down /etc/openvpn/update-resolv-conf" >> /home/ubuntu/openvpn-config/base.conf
echo "; Use these for systemd-resolved systems" >> /home/ubuntu/openvpn-config/base.conf
echo "; script-security 2" >> /home/ubuntu/openvpn-config/base.conf
echo "; up /etc/openvpn/update-systemd-resolved" >> /home/ubuntu/openvpn-config/base.conf
echo "; down /etc/openvpn/update-systemd-resolved" >> /home/ubuntu/openvpn-config/base.conf
echo "; down-pre" >> /home/ubuntu/openvpn-config/base.conf
echo "; dhcp-option DOMAIN-ROUTE ." >> /home/ubuntu/openvpn-config/base.conf

cd /home/ubuntu/openvpn-config
cp base.conf attacker.ovpn
echo -e "\n<ca>" >> attacker.ovpn
cat /etc/openvpn/server/ca.crt >> attacker.ovpn
echo -e "</ca>\n<cert>" >> attacker.ovpn
cat attacker.crt >> attacker.ovpn
echo -e "</cert>\n<key>" >> attacker.ovpn
cat attacker.key >> attacker.ovpn
echo -e "</key>\n<tls-auth>" >> attacker.ovpn
cat /etc/openvpn/server/ta.key >> attacker.ovpn
echo -e "</tls-auth>" >> attacker.ovpn

chown -R ubuntu.ubuntu /home/ubuntu/openvpn-config