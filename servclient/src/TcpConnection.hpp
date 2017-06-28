#ifndef TCPCONNECTION_HPP
#define TCPCONNECTION_HPP

#include <cstdint>
#include <string>
#include <sys/socket.h> // socklen_t
#include <unistd.h> // for close(filehandle)

/** Minimal C-Socket connection implementation. */
class TcpConnection {

  /** C file descriptor of the connection. */
  int _cconn;

  /** Endpoint address and port information. */
  std::string _peerip;
  uint16_t _peerport;

public:
  TcpConnection() : _cconn(-1) {}
  ~TcpConnection() { if (good()) close(_cconn); }

  /** Is used by a TcpSocket to set the accepted or established connection. */
  void setConnection(int c, const std::string& ip, uint16_t port);

  /** For checking, if the connection was successfully established. */
  bool good() const { return _cconn >= 0; }

  /** Sends message over the connection to endpoint. */
  bool send(const std::string& msg);

  /** Receives a message over the connection and return its size. */
  bool receive(std::string& msg);

  /** Returns a string with the IP of the other endpoint */
  const std::string& getPeerIpStr() const { return _peerip; }
  /** Returns the socket port at the other endpoint. */
  uint16_t getPeerPort() const { return _peerport; }
};

#endif /* TCPCONNECTION_HPP */
