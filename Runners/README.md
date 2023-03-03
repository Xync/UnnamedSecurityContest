These scripts use Python Paramkiko to:
1. Connect to a server via SSH
2. Upload a .tgz file and extract it to /tmp.  It is assumed that the .tgz contains a directory named remote/
3. Changes into the /tmp/remote/ directory and executes remote.sh

Remote.sh may be a consolidated script that runs many actions or may call other scripts bundled in the remote/ directory.

Each action/script:
* Must be written so that one failure does not cause the entire script to fail
* Should be written so that it can be run multiple times on the same server without causing errors
* Should work on Debian, Ubuntu, and Fedora versions

It is assumed that remote.sh will need root permissions.  The 'w_su' varient of the script assumes you know the root password and can 'su' to root using it.  The "w_sudo" varient assumes that the SSH user has sudo permissions.  The script will open a root shell using sudo before executing remote.sh, so this scripts and scripts that it calls do not need to include 'sudo' in front of other commands.

If needed, the command to create the tgz from a local directory name remote is (run this from the parent directory of remote)
  tar -czvf remote.tgz remote/

To run an attack:
python3 attack_linux_w_sudo.py 10.23.0.12 [user with sudo privs] [user's password] [remote.tgz]
