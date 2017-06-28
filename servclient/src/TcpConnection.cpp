#include "TcpConnection.hpp"

void TcpConnection::setConnection(int c, const std::string& ip, uint16_t port) { 
  if (c >= 0) {
    _cconn = c; 
    _peerip = ip;
    _peerport = port;
  }
}

bool TcpConnection::send(const std::string& msg) {
  ssize_t written = 0;
  ssize_t bytesMsg = msg.size() + 1;
  written = write(_cconn, msg.c_str(), bytesMsg);
  if (bytesMsg > written) 
    return false;
  else
    return true;
}

bool TcpConnection::receive(std::string& msg) {
  char buffer[1024];
  int res = read(_cconn, buffer, 1023);

  if (res < 0) 
    return false;
  else {
    msg = buffer;
    return true;
  }
}

