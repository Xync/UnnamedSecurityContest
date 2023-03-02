A collection of scripts that would be useful in during certain security contests.

!! Make sure to change the IP in Beachhead/watershell.sh to your IP as well as run `python3 -m http.server 80` while in the UnnamedSecurityContest root

The Private SSH key loaded will be distributed through another channel.

!! Beachhead - Install many access points - requires root access
 * adduser - Creates user scoring:redteam123
 * aptAPT - Installs several apt pre-execution scripts.  
   * changes the root password to redteam123
   * Modifies /etc/sshd/sshd_config to allow password login
   * Modifies /etc/sshd/sshd_config to allow remote root login
   * Loads a known SSH pubkey into authorized keys.
 * mksuidshell - Compiles a SUID shell and places it as /usr/bin/sushi, /usr/bin/bash, nologin, sh
 * PAMswap - Copies the PAM permit object over the deny object making failed logins work.
 * phonehome - Installs a debian package, which installs a go binary as a systemd service that runs whatever command is posted in a google cloud bucket.

!! Compiled Binaries
  * sshd - backdoored sshd daemon.  remote root login always allowed, password "redteam" accepted for any user.
  * ssh - client to activate a secondary backdoor which doesn't log logins.  https://github.com/Psmths/openssh-backdoor
  * sushi - precompiled suid shell
  * suidrun - runs the command pass as root (suitable for webshells) e.g. suidrun /bin/touch /tmp/win

!! Runners - Utilities to execute other scripts
  * attack_linux_w_su.py - Connects to a box with one user, su's to another user, executes all scripts in a tar.gz.  The scripts should be within a directory named 'remote'.  Requires python paramiko.  Requires a remote.sh to execute all the scripts.
  * attack_linux_w_sudo.py - Connects to a box with one user, executes all scripts in a tar.gz using sudo.  The scripts should be within a directory named 'remote'.  Requires python paramiko.
  * attack_master.py Loops through a file executing each command
  * a list of commands to be executed.  likely calling attack_linux...


