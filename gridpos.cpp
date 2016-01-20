/*!
  * @file gridpos.cpp
  */

#include "gridpos.hpp"

// Computes the hash value for a position of the grid.
// The hash value is the index of the given position in the uni-dimensional array built
// by laying out the grid positions row by row.
// The magic number '9' is equal to the dimension of the grid.
uint qHash(const GridPos &pos)
{
    return pos.row() * 9 + pos.column(); // 9 is the dimension of the grid's matrix
}
