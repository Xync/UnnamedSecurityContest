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

# A VPC to contain the servers.
resource "aws_vpc" "ccdc_vpc" {
  cidr_block = "10.23.0.0/24"
  tags = {
    Name = "CCDCVPC"
  }
}

# Subnet required for VPC
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

# Route to send VPC member's Internet traffic through the gateway
resource "aws_route" "outbound_inet_route" {
  route_table_id = "${aws_vpc.ccdc_vpc.main_route_table_id}"
  destination_cidr_block = "0.0.0.0/0"
  gateway_id = "${aws_internet_gateway.ccdc_gw.id}"
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
  subnet_id= aws_subnet.ccdc_subnet.id
  private_ip = "10.23.0.5"
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
/*
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

output "OpenVPN_IP" {
    value = aws_instance.OpenVPN.public_ip
}

output "Config_Command" {
  value = format("scp -i CCDCTest.pem ubuntu@%s:/home/ubuntu/openvpn-config/attacker.ovpn .", aws_instance.OpenVPN.public_ip)
}