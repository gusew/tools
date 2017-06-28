#include <iostream>
#include <string>
#include "TcpSocket.hpp"
#include "TcpConnection.hpp"

int main(int argc, char *argv[]) {
  unsigned connPort = 12345;
  std::string ipaddr("localhost");

  if (argc > 2) {
    // check for ip address and port number
    try {
      ipaddr = argv[1];
      connPort = std::stoi(argv[2]);
      if (connPort < 1 || connPort > 65535)
        throw "Invalid port number specified.";
    } catch (...) { 
      std::cerr << "Usage: " << argv[0] << " [<ip-addr> <port>]" << std::endl;
      std::cerr << "\tip-addr - IP addresss of server to connect to (default: localhost)" << std::endl;
      std::cerr << "\tport - port number of server (default: 12345)" << std::endl;
      return 1;
    }
  }
  
  // Create a socket
  TcpSocket sock;
  if (!sock.good()) {
    std::cerr << "Error: cannot create a socket" << std::endl;
    return 2;
  }
  
  // Connect to a remote server
  std::cout << "Connect to " << ipaddr << ":" << std::to_string(connPort) << "..." << std::endl;
  TcpConnection conn;
  if (!sock.connectTo(ipaddr, connPort, conn)) {
    std::cerr << "Error: connecting to server has failed." << std::endl;
    return 4;
  }
  std::cout << "Connected. Sending server a message..." << std::endl;

  if (!conn.send("Can you help me?")) {
    std::cerr << "Error: sending failed." << std::endl;
    return 5;
  }

  std::string recv;
  if (!conn.receive(recv)) {
    std::cerr << "Error: receiving failed." << std::endl;
    return 6;
  }
  std::cout << "Received: " << recv << std::endl;

  return 0;
}

