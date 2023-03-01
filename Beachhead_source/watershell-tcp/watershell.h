#ifndef WATERSHELL_H_
#define WATERSHELL_H_

// Networking
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// General
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>

class Watershell {
public:
  char iface[100];
  std::string gateway_mac;
  Watershell(int port);
  Watershell(int port, bool DEBUG);
  Watershell(int port, bool DEBUG, bool PROMISC, bool TCP_MODE);
  int RunOnce(void);
  void Init(void);

private:
  bool DEBUG, PROMISC, TCP_MODE;
  int port, sockfd;
  struct ifreq *sifreq;
  struct sock_fprog filter;

  void SetGatewayMAC(void);
  std::string GetMacFromIP(char *ip_addr);
  void GetInterfaceName(char iface[]);
  void SendReplyUDP(unsigned char *buf, const char *payload);
  void SendReplyTCP(unsigned char *buf, const char *payload);
  // void Sigint(int signum);
  void CalcIPChecksum(struct iphdr *ip);
};

/* its a datagram inside a packet inside a frame!
 * gotta be packed though!
 */
struct __attribute__((__packed__)) udpframe {
  struct ethhdr ehdr;
  struct iphdr ip;
  struct udphdr udp;
  unsigned char
      data[ETH_DATA_LEN - sizeof(struct udphdr) - sizeof(struct iphdr)];
};
/* its a datagram inside a packet inside a frame!
 * gotta be packed though!
 */
struct __attribute__((__packed__)) tcpframe {
  struct ethhdr ehdr;
  struct iphdr ip;
  struct tcphdr tcp;
  unsigned char
      data[ETH_DATA_LEN - sizeof(struct tcphdr) - sizeof(struct iphdr)];
};
#endif // WATERSHELL_H_
