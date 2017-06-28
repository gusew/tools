#include <iostream>
#include <sstream>
#include "TcpSocket.hpp"

int main(int argc, char** argv) {
  uint16_t listenPort = 12345;

  if (argc > 1) {
    // check for port number
    try {
      listenPort = std::stoi(argv[1]);
      if (listenPort == 0)
        throw "Invalid port number specified.";
    } catch (...) { 
      std::cerr << "Usage: " << argv[0] << " [<port>]" << std::endl;
      std::cerr << "\tport - port number for socket to listen on (default: 12345)" << std::endl;
      return 1;
    }
  }

  // Create a socket
  TcpSocket sock;
  if (!sock.good()) {
    std::cerr << "Error: cannot create a socket" << std::endl;
    return 2;
  }

  if (!sock.bindAndlisten(listenPort)) {
    std::cerr << "Error: binding socket and listening has failed." << std::endl;
    return 3;
  }

  // accept an incoming connection
  TcpConnection conn;
  if (!sock.acceptConnection(conn)) {
    std::cerr << "Error: accepting a connection has failed." << std::endl;
    return 4;
  }
  
  std::cout << "Connection accepted: " << conn.getPeerIpStr() << ":" << std::to_string(conn.getPeerPort()) << std::endl;

  std::string recv; // server first receives
  if (!conn.receive(recv)) {
    std::cerr << "Error: receiving failed." << std::endl;
    return 5;
  }
  std::cout << "Received: " << recv << std::endl;

  // then server sends a response
  if (!conn.send("No, sorry. Bye bye!")) {
    std::cerr << "Error: sending failed." << std::endl;
    return 6;
  }

  return 0;
}

