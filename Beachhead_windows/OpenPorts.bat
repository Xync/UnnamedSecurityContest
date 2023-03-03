  netsh advfirewall firewall add rule name="TCP Port 139" dir=in action=allow protocol=TCP localport=139
  netsh advfirewall firewall add rule name="TCP Port 445" dir=in action=allow protocol=TCP localport=445
  netsh advfirewall firewall add rule name="Remote Desktop" dir=in action=allow protocol=TCP localport=3389
  