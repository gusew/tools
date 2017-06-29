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

  /** readOnlyMode: true, if open only for read; if open also for write, then false */
  bool open(bool readOnlyMode);

  bool readLine(std::string& line);
  bool writeLine(const std::string& line);
};

#endif /* FILEHANDLER_HPP */
