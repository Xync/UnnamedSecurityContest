#!/usr/bin/bash

/usr/sbin/useradd -m ccdc_red
/usr/bin/echo -e "redteam123\nredteam123" | /usr/bin/passwd ccdc_red

mkdir /home/ccdc_red/.ssh/
echo "ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEArTU9+vjs6fHC+CeGD11a6vJhFLkQfM9FFwWEJ3BNHfFqS0QFJ0MjEBLD6V/5VjMIQkQG1kzp75qI0bEP18PFRDmI7IqIBBYDhoSlMsXMetnFmDMQA/nyJspubGQwotEE0NYOj00KmAhLYHscvc1ctY47rnST0cuoE2lVjRyVabUb8BcTm0v0wXPzS/fdaPqmXq/NsPFgARBKomK3w9AbJXCTDsOA3yNjE3nliE++imfN23By+1uoYVT+nAwzAiwXawhQfCtmOD2FisNzSCUmfE4TyqpmHaXUbKw1PZA7y5a8hl9hUaPJdzSQQj5YdsM2kRyFZKJYytGSh7V1IRlyiw== me@localhost" >> /home/ccdc_red/.ssh/authorized_keys

/usr/bin/chown ccdc_red.ccdc_red /home/ccdc_red/.ssh/
/usr/bin/chown ccdc_red.ccdc_red /home/ccdc_red/.ssh/authorized_keys
/usr/bin/chmod go-r /home/ccdc_red/.ssh/
/usr/bin/chmod go-r /home/ccdc_red/.ssh/authorized_keys

#Grant Sudoers access
/usr/bin/echo "ccdc_red ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers.d/ccdc


#Enable SSH
/usr/bin/systemctl enable ssh.service
/usr/bin/systemctl start ssh.service
