#!/usr/bin/bash 
apt install -y vsftpd ssl-cert --reinstall

useradd ftp_newowner
useradd ftp_chroot

#Fix up vsftpd.conf
sed -i 's/anonymous_enable=NO/anonymous_enable=YES/' /etc/vsftpd.conf
sed -i 's/#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf
sed -i 's/#anon_upload_enable=YES/anon_upload_enable=YES/' /etc/vsftpd.conf
sed -i 's/#anon_mkdir_write_enable=YES/anon_mkdir_write_enable=YES/' /etc/vsftpd.conf
sed -i 's/connect_from_port_20=YES/connect_from_port_20=NO/' /etc/vsftpd.conf
sed -i 's/#chown_uploads=YES/chown_uploads=YES/' /etc/vsftpd.conf
sed -i 's/#chown_username=whoever/chown_username=ftp_chroot/' /etc/vsftpd.conf
sed -i 's/#nopriv_user=ftp_newowner/nopriv_user=ftp_newowner/' /etc/vsftpd.conf

mkdir /srv/ftp/incomming/
chmod a+rw /srv/ftp/incomming/
systemctl enable vsftpd
systemctl start vsftpd
