#include "TcpSocket.hpp"
#include <cstring> // for memset
#include <sstream>
#include <netdb.h> // for gethostbyname

bool TcpSocket::bindAndlisten(uint16_t port) {
  if (!good()) return false; // socket creation failed

  // fill in the address structure containing self address
  struct sockaddr_in myaddr;
  memset(&myaddr, 0, sizeof(struct sockaddr_in)); // reset whole structure to zeros
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(port); // specify port
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind a socket to the address
  int res = bind(_csocket, (struct sockaddr*) &myaddr, sizeof(myaddr));
  if (res < 0) return false; // socket binding fails

  // set the "LINGER" timeout to zero, to close the listen socket immediately at program termination.
  struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
  setsockopt(_csocket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

  // listen for a connection
  res = listen(_csocket, 1); // "1" is the maximal length of the queue
  if (res < 0) return false; // listen failed

  _boundAndListens = true;
  return true;
}

bool TcpSocket::acceptConnection(TcpConnection& conn) {
  if (!good() && _boundAndListens) return false; // socket is not ready

  struct sockaddr_in cli_addr;
  socklen_t clilen;
  int cconn = accept(_csocket, (struct sockaddr*) &cli_addr, &clilen);
  if (cconn < 0) return false; // accepting failed
  
  std::string ipaddr(_convAddrToIpStr(cli_addr));
  uint16_t port(_convPeerPort(cli_addr));
  conn.setConnection(cconn, ipaddr, port);

  return true;
}

std::string TcpSocket::_convAddrToIpStr(const struct sockaddr_in& addr) const {
  std::stringbuf b;
  std::ostream bos(&b);

  bos << std::to_string((ntohl(addr.sin_addr.s_addr) >> 24) & 0xff) << "." << // high byte of addr
    std::to_string((ntohl(addr.sin_addr.s_addr) >> 16) & 0xff) << "." << 
    std::to_string((ntohl(addr.sin_addr.s_addr) >> 8) & 0xff) << "." <<
    std::to_string(ntohl(addr.sin_addr.s_addr) & 0xff); // low byte of addr
  
  return b.str();
}

uint16_t TcpSocket::_convPeerPort(const struct sockaddr_in& addr) const { 
  return ntohs(addr.sin_port); 
}

bool TcpSocket::connectTo(const std::string& ip, uint16_t port, TcpConnection& conn) {
  // convert ip address to C-structure
  struct sockaddr_in peeraddr;
  memset(&peeraddr, 0, sizeof(peeraddr));
  // resolve the server address (convert from symbolic name to IP number)
  struct hostent* host = gethostbyname(ip.c_str());
  if (host == nullptr) return false; // failed to convert host address // TODO check if correct to return here
  peeraddr.sin_family = AF_INET;

  // set port number
  peeraddr.sin_port = htons(port); // specify port

  // write resolved IP address of a server to the address structure
  memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

  int res = connect(_csocket, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
  if (res < 0) return false; // connect has failed

  conn.setConnection(_csocket, ip, port);
  return true;

}
