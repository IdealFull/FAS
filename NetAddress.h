#ifndef SOCKETNETADDRESS_H
#define SOCKETNETADDRESS_H
#include <netinet/in.h>
#include <arpa/inet.h>

class NetAddress
{
private:
  struct sockaddr_in addr;
public:
  NetAddress(unsigned short sa_family, int sa_port, const char *sa_ip);
  NetAddress(unsigned short sa_family, int sa_port, unsigned int sa_ip);
  NetAddress(int sa_port, const char *sa_ip);
  NetAddress(int sa_port);
  struct sockaddr_in& NetAddress_get_addr();
  socklen_t NetAddress_get_addr_len();
};

#endif // SOCKETNETADDRESS_H
