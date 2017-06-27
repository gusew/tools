#include "Grid.hpp"
  
bool Grid::getNeighbors(unsigned x, unsigned y, GridContainer& neigh) const {
  if (!coordValid(x, y)) return false;

  // for wraparound
  unsigned above = (y == 0 ? _size-1 : y-1);
  unsigned right = (x == _size-1 ? 0 : x+1);
  unsigned below = (y == _size-1 ? 0 : y+1);
  unsigned left = (x == 0 ? _size-1 : x-1);

  neigh.clear();
  // ordering is clockwise: above, right, below, left
  neigh.push_back(getCell(x, above));
  neigh.push_back(getCell(right, y));
  neigh.push_back(getCell(x, below));
  neigh.push_back(getCell(left, y));

  return true;
}

bool Grid::setCell(unsigned x, unsigned y, double value) {
  if (coordValid(x, y)) {
    _cells[coordToIdx(x, y)] = value;
    return true;
  }
  else
    return false;
}

bool Grid::fillLine(const GridContainer& values) {
  // check first if not already filled and whether enough values given for complete line
  if (isFilled() || values.size() < _size) return false;
  
  for (auto itr(values.cbegin()); itr != values.cend(); ++itr)
    _cells.push_back(*itr);

  return true;
}


#ifdef UNIT_TEST
#include <cassert>
#include <iostream>

int main() {
  // minimal grid
  Grid g1(0);
  assert(g1.getSize() == 0);
  try {
    g1.getCell(0, 0);
    assert(false);
  } catch (const char* ex) {}
  
  GridContainer gc1;
  assert(!g1.getNeighbors(0, 0, gc1));

  assert(!g1.setCell(0, 0, 1.0));
  assert(!g1.fillLine(gc1));

  // resonable grid size
  Grid g2(3);
  assert(g2.getSize() == 3);

  try {
    g2.getCell(0, 0);
    assert(false);
  } catch (const char* ex) {}
  
  GridContainer gc2;
  assert(!g2.getNeighbors(0, 0, gc2));
  assert(!g2.setCell(0, 0, 1.0));
  assert(!g2.fillLine(gc2));

  gc2.push_back(1.0); gc2.push_back(2.0); gc2.push_back(3.0);
  assert(g2.fillLine(gc2));

  try {
    g2.getCell(0, 0);
    assert(false);
  } catch (const char* ex) {}
  
  gc2.clear();
  gc2.push_back(10.0); gc2.push_back(11.0); gc2.push_back(12.0);
  assert(g2.fillLine(gc2));

  try {
    g2.getCell(0, 0);
    assert(false);
  } catch (const char* ex) {}
  
  gc2.clear();
  gc2.push_back(-1.234); gc2.push_back(-10.10100100); gc2.push_back(-68.171819);
  assert(g2.fillLine(gc2));

  assert(g2.getSize() == 3);
  assert(g2.getCell(0, 0) == 1.0); assert(g2.getCell(1, 0) == 2.0); assert(g2.getCell(2, 0) == 3.0);
  assert(g2.getCell(0, 1) == 10.0); assert(g2.getCell(1, 1) == 11.0); assert(g2.getCell(2, 1) == 12.0);
  assert(g2.getCell(0, 2) == -1.234); assert(g2.getCell(1, 2) == -10.10100100); assert(g2.getCell(2, 2) == -68.171819);

  try {
    g2.getCell(3, 0);
    assert(false);
  } catch (const char* ex) {}
  try {
    g2.getCell(0, 3);
    assert(false);
  } catch (const char* ex) {}
  
  assert(g2.getNeighbors(0, 0, gc2));
  assert(gc2.size() == 4);
  assert(gc2[0] == -1.234);
  assert(gc2[1] == 2.0);
  assert(gc2[2] == 10.0);
  assert(gc2[3] == 3.0);
 
  assert(g2.getNeighbors(1, 1, gc2));
  assert(gc2.size() == 4);
  assert(gc2[0] == 2.0);
  assert(gc2[1] == 12.0);
  assert(gc2[2] == -10.10100100);
  assert(gc2[3] == 10.0);

  assert(g2.getNeighbors(2, 2, gc2));
  assert(gc2.size() == 4);
  assert(gc2[0] == 12.0);
  assert(gc2[1] == -1.234);
  assert(gc2[2] == 3.0);
  assert(gc2[3] == -10.10100100);

  std::cout << "Unit tests executed successfully." << std::endl;
  return EXIT_SUCCESS;
}
#endif


