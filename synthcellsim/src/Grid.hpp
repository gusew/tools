#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

/** Container type for storing cell values. */
typedef std::vector<double> GridContainer;

/** Represents a quadratic grid of values. */
class Grid {
  /** Stores the size of the grid in one dimension. */
  unsigned _size;
  
  /** Stores a value for each cell. */
  GridContainer _cells;

  inline unsigned coordToIdx(unsigned x, unsigned y) const { return (x + y*_size); }
  inline bool isFilled() const { return (_cells.size() == _size*_size); }
  inline bool coordValid(unsigned x, unsigned y) const { return (isFilled() && x < _size && y < _size); }

public:
  Grid(unsigned size) : _size(size) {}
  
  /** Size member field is read-only. */
  inline unsigned getSize() const { return _size; }

  /** Return the value of specified cell. */
  inline double getCell(unsigned x, unsigned y) const { if (coordValid(x, y)) return _cells[coordToIdx(x, y)]; else throw "Exception: Invalid cell parameters."; }

  /** Returns the neighbors of a specified cell (over borders). */
  bool getNeighbors(unsigned x, unsigned y, GridContainer& neigh) const;

  /** Is used for filling the grid with data. Returns false, if Grid is already completely filled. */
  bool fillLine(const GridContainer& values);

  /** Initializes a grid with zero values. */
  void initZeros() { _cells.clear(); for (unsigned i=0; i < _size*_size; ++i) _cells.push_back(0); };

  /** Use this for changing an already existing value in the grid. */
  bool setCell(unsigned x, unsigned y, double value);

  /** Swap contents with another grid instance. */
  void swapCells(Grid& oth) { _cells.swap(oth._cells); }
};

#endif /* GRID_HPP */
