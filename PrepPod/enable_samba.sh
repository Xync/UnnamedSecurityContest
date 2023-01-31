##!/usr/bin/bash

if [ ! -f "updated.txt" ]; then
    ./apt_update_and_flag.sh
fi


apt install -y samba
mkdir /srv/samba_share
chmod a+rwx /srv/samba_share


cat <<EOF | sudo tee -a /etc/samba/smb.conf
[share]
  comment = Share Directory
  path = /srv/samba_share
  writeable = yes
  public = yes
  guest ok = yes
EOF


systemctl restart smbd.service
