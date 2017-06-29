#include "FileHandler.hpp"
#include <iostream>

bool FileHandler::open(bool readOnlyMode) {
  if (readOnlyMode)
    _handle.open(_path, std::fstream::in);
  else
    _handle.open(_path, std::fstream::out | std::fstream::trunc);

  if (!_handle.is_open()) { // file does not exist
    std::cerr << "Failed to open the file: " << _path << " (no permissions?)." << std::endl;
    return false;
  }
  else 
    return true;
}

bool FileHandler::readLine(std::string& line) {
  // check input file stream for validity
  if (!_handle.good()) return false; // some errors occured or file is empty

  std::getline(_handle, line);
  return true;
}

bool FileHandler::writeLine(const std::string& line) {
  // check output file stream for validity
  if (_handle.bad()) return false; // some errors occured

  _handle << line << std::endl;

  return true;
}



