#!/usr/bin/bash

terraform apply -auto-approve > apply.output
#terraform destroy -target aws_instance.OpenVPN
#terraform destroy -target aws_instance.Kali
#terraform destroy -target aws_instance.Ubuntu16
#terraform destroy -target aws_instance.Ubuntu18
#terraform destroy -target aws_instance.Ubuntu20
#terraform destroy -target aws_instance.Centos8
#terraform destroy -target aws_instance.Debian9
#terraform destroy -target aws_instance.Debian11
#terraform destroy -target aws_instance.Fedora32

#Everything except openvpn and kali
terraform destroy -auto-approve -target aws_instance.Ubuntu12 -target aws_instance.Ubuntu14 -target aws_instance.Ubuntu16 -target aws_instance.Ubuntu18 -target aws_instance.Ubuntu20 -target aws_instance.CentOS7 -target aws_instance.Centos8 -target aws_instance.Deb8 -target aws_instance.Debian9 -target aws_instance.Debian11 -target aws_instance.Fedora32 > destroy.output
ssh-keygen -f "/root/.ssh/known_hosts" -R "10.23.0.100"

cat apply.output
