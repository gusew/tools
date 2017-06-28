#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h> // for IPPROTO_TCP
#include <unistd.h> // for close(filehandle), gethostname
#include "TcpConnection.hpp"
//#include <cstdio>

/** Minimal C-Socket implementation for TCP connections. */
class TcpSocket {

  /** C file descriptor of the socket. */
  int _csocket;

  /** Is set to true, after the socket is bound and configured for listening. */
  bool _boundAndListens;

  /** Helper function to convert address information of an established connection to an IP string. */
  std::string _convAddrToIpStr(const struct sockaddr_in& addr) const;
  /** Helper function to convert connection information to a port number. */
  uint16_t _convPeerPort(const struct sockaddr_in& addr) const;

public: 
  TcpSocket() : _csocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)), _boundAndListens(false) {} // AF_INET6, SOCK_DGRAM
  ~TcpSocket() { if(good()) close(_csocket); }

  /** For checking, if socket was successfully created. */
  bool good() const { return _csocket >= 0; }

  /** Bind the socket to a port and set it as a listener. */
  bool bindAndlisten(uint16_t port);

  /** Accept incoming connections on the socket. */
  bool acceptConnection(TcpConnection& conn);

  /** Connect to a server and return established connection on success. */
  bool connectTo(const std::string& ip, uint16_t port, TcpConnection& conn);
};

#endif /* TCPSOCKET_HPP */
