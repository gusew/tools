#ifndef GRIDSERIALIZER_HPP
#define GRIDSERIALIZER_HPP

#include <string>
#include "Grid.hpp"

struct GridSerializer {
  /** Read grid data from file and fill grid structure. */
  static bool deserialize(const std::string& file, Grid& g);

  /** Store grid data from given grid instance to specified file. */
  static bool serialize(const Grid& g, const std::string& file);
};

#endif /* GRIDSERIALIZER_HPP */
