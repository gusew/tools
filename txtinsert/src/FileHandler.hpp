#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <fstream>
#include <string>

class FileHandler {

  std::string _path;
  std::fstream _handle;

public: 
  FileHandler(const std::string& filepath) : _path(filepath), _handle() {}
  ~FileHandler() { if (_handle.is_open()) _handle.close(); }

  bool openread();
  bool openwrite();

  inline std::fstream& handle() noexcept { return _handle; }
};

#endif /* FILEHANDLER_HPP */
