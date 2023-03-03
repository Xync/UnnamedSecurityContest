Create an AWS Access key (under IAM console).
  Select service IAM 
    Click on Users in the left tab
    Create a new user if necessary
      Provide a username.  Console access is not required.
      Grant permissions assigning to a group.  If necessary create a group
        Group name can be anything.  Grant the "AmazonEC2FullAccess" policy
      Confirm the settings
    Click on the user name.
    Click on the "Security credentials" tab
    Click on "Create Access Key"
      Select CLI
      Provide a description
    The access keys will be displayed.  Store these values in a password vault.

In the shell you'll run vagrant from, export two environment variables with the above variables.
  AWS_ACCESS_KEY
  AWS_SECRET_ACCESS_KEY
On Windows the batch file "setcreds.bat" will prompt you for the values and set the as variables.
  Linux users can handle this on their own.

Create a AWS key pair
  Select service EC2
  Make sure you are in the US-West-2 (Oregon) region (top right)
  Click on "Key Pairs" in the top main window or in the left hand nav under Network & Security
   named "CCDCTest" (Under EC2 console)
  Click "Create key pair"
    Enter the name CCDCTest.  If you use a different name you'll have to update the terraform file.
    Seect .pem format
  Download the pem file and put it in the /TerraformTargets directory

Install OpenVPN via package manager or download from https://openvpn.net/community-downloads/

Be aware that there are a set of bootstrap files that give each server a basic configuration.
If you want, you could take these bootstrap files to your own VMs

Run once in the project directory
terraform init 

Before you can build the CentOS, Fedora, or Kali servers you have to agree to a license.
The page is for AMIs with license charges, but there are none (except Ubuntu 14??).  Only normal hardware costs.
Visit this URL while logged in to the console.
Kali - https://aws.amazon.com/marketplace/pp?sku=7lgvy7mt78lgoi4lant0znp5h
Ubuntu 14 - https://aws.amazon.com/marketplace/pp?sku=cvleqruhfg3cyb1cvq7iu0mvg
Centos 7 - https://aws.amazon.com/marketplace/pp?sku=cvugziknvmxgqna9noibqnnsy
If you uncomment later servers, you have to accept these too.
Fedora 32 - https://aws.amazon.com/marketplace/pp?sku=633jhlnyl61qp9ukyefuy0a07
Centos 8 - https://aws.amazon.com/marketplace/pp?sku=9svc856m58q4n42iz9he7imlb


** Build the environment.
terraform apply

By default, OpenVPN and SSH will be allowed from anywhere to the OpenVPN server (the only one with a public IP)
If you want to restrict this to your current IP use this syntax instead of the above
terraform apply -var='allowed_sources=["97.118.152.240/32"]'

When you are done, destroy the environment (stop incurring charges)
terraform destroy

After running the script, note the OpenVPN_IP and copy the OpenVPN config to your machine
scp -i CCDCTest.pem ubuntu@[OpenVPN_IP]:/home/ubuntu/client-configs/attacker.ovpn .
Import the config into OpenVPN and connect

In a full build the servers will be as follows
OpenVPN - 10.23.0.5
Kali - 10.23.0.100
Centos 7 Stream - 10.23.0.7
(inactive) Centos 8 Stream - 10.23.0.8
Debian 8 - 10.23.0.8
(inactive) Debian 9 - 10.23.0.9
(inactive) Debian 11 - 10.23.0.11
Ubuntu 12 - 10.23.0.12
Ubuntu 14 - 10.23.0.14
(inactive) Ubuntu 16 - 10.23.0.16
(inactive) Ubuntu 18 - 10.23.0.18
(inactive) Ubuntu 20 - 10.23.0.20
Windows 2012 PDC - 10.23.0.30
Windows 2016 Domain Member - 10.23.0.31
Fedora 32 - 10.23.0.32


Every Linux server has at these two users:
johndoe:RedTeam123 - Regular user with sudo privs
root:Secure123

Windows Local Admin password is displayed on build.
Domain Users:
WB\DomAdmin:DAPassword1!
WB\User1:Password1!

Password based ssh authentication is enabled for users and root.
Keep in mind that every time you destroy and rebuild the environment, you'll get a new IP 
for the OpenVPN server and the host keys will change on each target.  Taking a backup of your known_hosts

You can use the CCDCTest key to log in to these systems and sudo to root as well.
For Debian 8 the username is 'debian'
For Debian 9-11 the username is 'admin'
For Ubuntu the username is 'ubuntu'
For Fedora the username is 'fedora'
For Centos the username is 'centos'
e.g. ssh -i CCDCTest.pem fedora@10.23.0.32

After building the network infrastructure and the OpenVPN server, you can create or destroy specific servers using this syntax:
terraform apply -target=aws_instance.Ubuntu18
terraform destroy -target=aws_instance.Ubuntu18

