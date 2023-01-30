#!/usr/bin/bash

if [ ! -f "updated.txt" ]; then
    ./apt_update_and_flag.sh
fi

apt install -y apache2


echo "<HTML><BODY>GO AWAY!</BODY></HTML>" > /var/www/html/index.html
chmod go+r /var/www/html/index.html

systemctl start apache2
