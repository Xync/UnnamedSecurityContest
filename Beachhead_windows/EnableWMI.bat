netsh advfirewall firewall set rule group="windows management instrumentation (wmi)" new enable=yes
netsh advfirewall firewall add rule dir=in name="DCOM" program=%systemroot%\system32\svchost.exe service=rpcss action=allow protocol=TCP localport=135
netsh advfirewall firewall add rule dir=in name ="WMI" program=%systemroot%\system32\svchost.exe service=winmgmt action = allow protocol=TCP localport=any
netsh advfirewall firewall add rule dir=in name ="UnsecApp" program=%systemroot%\system32\wbem\unsecapp.exe action=allow
