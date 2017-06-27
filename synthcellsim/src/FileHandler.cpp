#include "FileHandler.hpp"

bool FileHandler::openread() {
  // open in read mode
  _handle.open(_path, std::fstream::in);
  return _handle.is_open();
}

bool FileHandler::openwrite() {
  _handle.open(_path, std::fstream::out | std::fstream::trunc);
  return _handle.is_open();
}

