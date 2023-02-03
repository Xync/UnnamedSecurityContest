# --- Configuration Variables ---
# List the external IP or subnet that you will be connecting from.  Multiple subnets may be listed.
variable "allowed_sources" {
    type = list(string)
    default = ["0.0.0.0/0"]
    #default = ["73.95.243.33/32"]
    #default = ["97.118.152.240/32"]
}

# --------------------------------
provider "aws" {
  # Run setcreds.bat prior to execution
  region     = "us-west-2"
}

# A VPC to contain the servers and gateways
resource "aws_vpc" "ccdc_vpc" {
  cidr_block = "10.23.0.0/16"
  tags = {
    Name = "CCDCVPC"
  }
}

# Subnet for the gateways (public)
resource "aws_subnet" "ccdc_gw_subnet" {
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  cidr_block = "10.23.1.0/24"
  availability_zone = "us-west-2c"
  tags = {
    Name = "ccdcSubnet"
  }
}

# Subnet for the servers
resource "aws_subnet" "ccdc_subnet" {
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  cidr_block = "10.23.0.0/24"
  availability_zone = "us-west-2c"
  tags = {
    Name = "ccdcSubnet"
  }
}

# Internet Gateway required to allow members of the VPC to get to the Internet
resource "aws_internet_gateway" "ccdc_gw" {
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  tags = {
    Name = "ccdcGateway"
  }
}

# Elastic IP for the NAT gateway
resource "aws_eip" "ccdc_nat_eip" {
  vpc = true
}

resource "aws_nat_gateway" "ccdc_nat_gw" {
  allocation_id = "${aws_eip.ccdc_nat_eip.id}"
  subnet_id = "${aws_subnet.ccdc_gw_subnet.id}"
  tags = {
    Name = "CCDC NAT Gateway"
  }
}

resource "aws_route_table" "nat_to_gw_route" {
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = aws_internet_gateway.ccdc_gw.id
  }
}

resource "aws_route_table_association" "nat_route_assoc" {
  subnet_id = "${aws_subnet.ccdc_gw_subnet.id}"
  route_table_id = aws_route_table.nat_to_gw_route.id
}

# Route to send VPC member's Internet traffic through the NAT gateway
/*resource "aws_route" "outbound_inet_route" {
  route_table_id = "${aws_vpc.ccdc_vpc.main_route_table_id}"
  destination_cidr_block = "0.0.0.0/0"
  gateway_id = "${aws_nat_gateway.ccdc_nat_gw.id}"
  #gateway_id = "${aws_internet_gateway.ccdc_gw.id}"
}*/
resource "aws_route_table" "servers_to_nat_route" {
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  route {
    cidr_block = "0.0.0.0/0"
    gateway_id = aws_nat_gateway.ccdc_nat_gw.id
  }
}

resource "aws_route_table_association" "server_route_assoc" {
  subnet_id = "${aws_subnet.ccdc_subnet.id}"
  route_table_id = aws_route_table.servers_to_nat_route.id
}

# Security Group to allow:
#  SSH traffic from "allowed source" subnet
resource "aws_security_group" "CCDC_sg"{
  name = "All from specific network"
  description = "Allow all traffic in from a specific network"
  vpc_id = "${aws_vpc.ccdc_vpc.id}"
  tags = {
    Name = "CCDC_SecurityGroup"
  }

  ingress {
    from_port = 0
    to_port = 0
    protocol = "-1"
    cidr_blocks = var.allowed_sources
  }
  ingress {
    from_port = 0
    to_port = 0
    protocol = "-1"
    cidr_blocks = ["10.23.0.0/24"]
  }
  egress {
    from_port = 0
    to_port = 0
    protocol = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

# OpenVPN Access Server
resource "aws_instance" "OpenVPN" {
  # Ubuntu 22.04 LTS
  ami           = "ami-03f8756d29f0b5f21"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_gw_subnet.id
  private_ip = "10.23.1.5"
  associate_public_ip_address = true
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_openvpn.sh")}"
}

# Kali "Attack" Pod
# To test pod setup scripts.  Attack scripts can be run directly from your host machine.
resource "aws_instance" "Kali" {
  # Kali's official AMI
  ami           = "ami-07a53017d32cd4da0"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.100"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  #user_data = "${file("bootstrap_linux.sh")}"
}
/*
# Ubuntu 12.04 LTS
resource "aws_instance" "Ubuntu12" {
  ami           = "ami-830c94e3"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.12"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
*/
# Ubuntu 14.04 LTS
resource "aws_instance" "Ubuntu14" {
  ami           = "ami-02ced4f4da4322f56"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.14"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
/*
# Debian 8.5
resource "aws_instance" "Deb8" {
  ami           = "ami-094b27f54904c24ce"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.8"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# CentOS 7
resource "aws_instance" "CentOS7" {
  ami           = "ami-08c191625cfb7ee61"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.7"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
*/
/*
# CAN'T FIND A FEDORA 21 AMI!
# Fedora 21
resource "aws_instance" "Fedora21" {
  # This is actually a docker test build but should work
  ami           = "ami-f7256fc7"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.21"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
*/
/*
# Fedora 32
resource "aws_instance" "Fedora32" {
  ami           = "ami-04e340b8978593202"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.32"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
*/
/* Newer boxes not expected to be part of the environment
# Ubuntu 20.04 LTS
resource "aws_instance" "Ubuntu20" {
   ami           = "ami-0d31d7c9fc9503726"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.20"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# Ubuntu 18.04 LTS
resource "aws_instance" "Ubuntu18" {
  ami           = "ami-0135afc6d226a70a4"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.18"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# Ubuntu 16.04 LTS
resource "aws_instance" "Ubuntu16" {
  ami           = "ami-0db01fabda16f6445"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.16"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# Debian 11
resource "aws_instance" "Debian11" {
  ami           = "ami-05063446e767da4ff"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.11"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# Debian 9 - Stretch
resource "aws_instance" "Debian9" {
  ami           = "ami-0f3a8a1e9a9a8937f"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.9"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}

# Centos 8 - Stream
resource "aws_instance" "Centos8" {
  ami           = "ami-031e6a417aae9b9f6"
  instance_type = "t2.micro"
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.8"
  associate_public_ip_address = false
  vpc_security_group_ids = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  user_data = "${file("bootstrap_linux.sh")}"
}
*/

# Couldn't figure out how to get the AWS generated Admin pass
# Since self references were forbidden.  So I'll create a new one
resource "random_string" "admin_pass" {
  length = 10
}

# Define the Primary Domain Controller
resource "aws_instance" "pdc" {
  # Base Win2012 Image
  ami           = "ami-037a58d5f7db56631"
  # A t2.micro takes ~20 minutes for the domain to be available
  # A t2.micro takes ~15 minutes for the domain to be available
  # A t2.medium takes ~10 minutes for the domain to be available
  instance_type = "t2.small"
  subnet_id= "${aws_subnet.ccdc_subnet.id}"
  private_ip = "10.23.0.30"
  associate_public_ip_address = false
  security_groups = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  #get_password_data = true
  # This script will be executed once the instance is available
  # In this case, configure the system as a domain controller
  # Since this can't be done without a reboot in the middle
  # Set up a scheduled task to complete the configuration
  user_data = <<-EOF
    <powershell>
    #echo "User Data Started" | Out-File -filepath C:\Users\Administrator\log.txt
    # Install Active Directory Powershell Tools
    Add-windowsfeature AD-Domain-Services -IncludeManagementTools
    # Create User Creation Script (Step 2)
echo @'
  #echo adduser_started | Out-File -filepath C:\Users\Administrator\log.txt -append
  New-ADUser -SamAccountName DomAdmin -Name "Domain Admin" -UserPrincipalName domadmin@wb.local -AccountPassword (ConvertTo-SecureString -asplaintext "DAPassword1!" -force) -Enabled $true -PasswordNeverExpires $true -Path "CN=Users,DC=wb,DC=local"
  Add-ADGroupMember -Identity "Domain Admins" -Members DomAdmin
  Add-ADGroupMember -Identity "Remote " -Members DomAdmin
  New-ADUser -SamAccountName User1 -Name "User One" -UserPrincipalName user1@wb.local -AccountPassword (ConvertTo-SecureString -asplaintext "Password1!" -force) -Enabled $true -Path "CN=Users,DC=wb,DC=local"
  Rename-Computer -NewName "PDC"
  # Delete the scheduled task and undo auto-login
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name AutoAdminLogon -Force | Out-Null
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name DefaultUserName -Force | Out-Null
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon"  -Name DefaultPassword -Force | Out-Null
  SCHTASKS.EXE /DELETE /F /TN "Step2"
  del C:\Users\Administrator\addusers.ps1
  echo "Calling Reboot 2" | Out-File -filepath C:\Users\Administrator\log.txt -append
  Restart-Computer -Force
'@ | Out-File -filepath C:\Users\Administrator\addusers.ps1

    # Set the admin pass to our random string
    $user = [adsi]"WinNT://localhost/Administrator,user"
    $user.SetPassword("${random_string.admin_pass.result}")
    $user.SetInfo()

    # Set the system to automatically log in and create a schedule task for step 2
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name AutoAdminLogon -PropertyType DWORD -Value 1 -Force | Out-Null
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name DefaultUserName -PropertyType String -Value Administrator -Force | Out-Null
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon"  -Name DefaultPassword -PropertyType String -Value "${random_string.admin_pass.result}" -Force | Out-Null
    SCHTASKS.EXE /CREATE /F /SC ONLOGON /DELAY 0001:00 /TN "Step2" /RL HIGHEST /TR "Powershell.exe -ExecutionPolicy Bypass -File C:\Users\Administrator\addusers.ps1"

    # Actually do the domain promotion.  This will cause a reboot automatically
    install-addsforest -domainname wb.local -DomainNetBIOSName "WB" -Force -safemodeadministratorpassword (convertto-securestring "SafePass1!" -asplaintext -force)
    </powershell>
    EOF
}

# Define Domain Member 1 (Win2016 Server)
resource "aws_instance" "Win2016" {
  ami           = "ami-005b3f1e9bd7a715a"
  instance_type = "t2.micro"
  subnet_id= "${aws_subnet.ccdc_subnet.id}"
  private_ip = "10.23.0.31"
  associate_public_ip_address = false
  security_groups = [aws_security_group.CCDC_sg.id]
  key_name="CCDCTest"
  get_password_data = true
  # This script will be executed once the instance is available
  # In this case, configure the system as a domain controller
  # Since this can't be done without a reboot in the middle
  # Set up a scheduled task to complete the configuration
  user_data = <<-EOF
    <powershell>
    # Create Join Domain Script (Step 2)
echo @'
  # Delete the scheduled task and undo auto-login
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name AutoAdminLogon -Force | Out-Null
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name DefaultUserName -Force | Out-Null
  Remove-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon"  -Name DefaultPassword -Force | Out-Null
  SCHTASKS.EXE /DELETE /F /TN "Step2"
  del C:\Step2.ps1
  # Wait for the PDC to respond to the DNS lookup
  do {
    sleep 15
    $date = Get-Date
    echo "Checking for lookup at $date"
    ipconfig /flushdns
    $res = ping pdc.wb.local
  } until ($?)
  netdom JOIN $env:computername /Domain:wb.local /UserD:WB\domadmin /PasswordD:DAPassword1! /reboot > C:\result.txt
'@ | Out-File -filepath C:\Step2.ps1
    # Simplify the file path above because 2003 doesn't have a Users directory

    # Set the admin pass to our random string
    $user = [adsi]"WinNT://localhost/Administrator,user"
    $user.SetPassword("${random_string.admin_pass.result}")
    $user.SetInfo()

    # Set the system to automatically log in and create a schedule task for step 2
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name AutoAdminLogon -PropertyType DWORD -Value 1 -Force | Out-Null
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon" -Name DefaultUserName -PropertyType String -Value Administrator -Force | Out-Null
    New-ItemProperty -Path "HKLM:\Software\Microsoft\Windows NT\CurrentVersion\winlogon"  -Name DefaultPassword -PropertyType String -Value "${random_string.admin_pass.result}" -Force | Out-Null
    SCHTASKS.EXE /CREATE /F /SC ONLOGON /DELAY 0001:00 /TN "Step2" /RL HIGHEST /TR "Powershell.exe -ExecutionPolicy Bypass -File C:\Step2.ps1"

    # Point the DNS to the PDC or domain join won't work
    # Win2008 version
    #netsh interface ip set dns "Local Area Connection" static 10.23.0.30
    # Powershell 3 version (probably don't need both but it works)
    Set-DnsClientServerAddress -InterfaceAlias "Ethernet" -ServerAddresses ("10.23.0.30")
    # Rename the computer
    Rename-Computer -NewName Server1
    # Reboot to take effect and trigger step 2
    Restart-Computer -Force
    </powershell>
    EOF
}


output "PDC_Info" {
  value = "PDC Password: ${random_string.admin_pass.result}\n"
}

output "OpenVPN_IP" {
    value = aws_instance.OpenVPN.public_ip
}

output "Config_Command" {
  value = format("scp -i CCDCTest.pem ubuntu@%s:/home/ubuntu/openvpn-config/attacker.ovpn .", aws_instance.OpenVPN.public_ip)
}

output "Windows Timing" {
  value = "The Windows Domain Members will not join the domain for up to 20 minutes because the PDC takes that long to complete configuration."
}