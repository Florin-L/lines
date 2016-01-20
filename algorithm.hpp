/*!
  * @file algorithm.hpp
  */

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <cmath>
#include <set>
#include "gridpos.hpp"

class GridItem;

/*! \brief This class encapsulates the implementation of the A* algorithm for finding the path
  * between two squares in the grid.
  */
class Algorithm
{
public:
    /*! \brief Wraps the coordinates and the cost of a square.
      *
      */
    struct PathCost
    {
        PathCost(GridPos gridPos, int cost = 0)
                : m_gridPos(gridPos), m_cost(cost)
        {
        }

        GridPos m_gridPos;
        int m_cost;
    };

    /*! \brief The functor used to sort the elements of the openlist.
     *
     */
    struct CmpCostPath
    {
        bool operator()(const PathCost &pthc1, const PathCost &pthc2)
        {
            return pthc1.m_cost < pthc2.m_cost;
        }
    };

    // type definitions
    typedef std::multiset<PathCost, CmpCostPath> OpenSet_t;
    typedef OpenSet_t::iterator OpenSetIter_t;
    typedef OpenSet_t::const_iterator OpenSetConstIter_t;

    //
    // helper methods
    //

    /*! \brief Searches for a the presence of a square in the openset using the square's coordinates.
      *
      * The openset is implemented as a multiset. The multiset is based on a weak-order which
      * in our case is given by the current cost of a square in the shortest path.
      * In order to check out whether a certain square was considered we have to sequantially
      * look for it within the multiset based on the square's coordinates
      * (the find method from the std::multiset is based on the
      * function that gives the weak-order).
      *
      * @param list The openlist.
      * @param coords The coordinates of the square.
      * @return If the square is found then its position in openlist is returned; otherwise returns -1.
      */
    static int findElementInSet(const OpenSet_t &list,
                                const GridPos &coords)
    {
        OpenSetConstIter_t itb = list.begin();
        OpenSetConstIter_t ite = list.end();

        int pos = 0;
        for (OpenSetConstIter_t it = itb ; it != ite; ++it) {
            if (const_cast<GridPos&>(coords) == it->m_gridPos) {
                return pos;
            }
            ++pos;
        }

        return -1;
    }

    static void dumpOpenSet(OpenSet_t list)
    {
        OpenSetIter_t itb = list.begin();
        OpenSetIter_t ite = list.end();

        qDebug().nospace() << "*****";
        qDebug() << "openset:";
        for (OpenSetIter_t it = itb ; it != ite; ++it) {
            qDebug() << it->m_gridPos;
        }
        qDebug() << "*****";
    }
    //

public:
    /*! Implementation of the A* algorithm to find the shortest path between the start and the end position in grid.
      *
      * @param grid     The grid we searche the path within.
      * @param beginPos The starting position on the grid.
      * @param endPos   The ending position on the grid.
      * @param path     The shortest path between the given coordinates.
      */
    static bool shortestPath(GridItem *, GridPos &, GridPos &, QList<GridPos> &);

    /*!
      * @param from
      * @param currentNode
      * @param path
      */
    static void reconstructPath(int *, int, QList<GridPos> &);

    /*! Computes the Manhattan estimation between two squares of the grid.
      *
      * @param pt1
      * @param pt2
      */
    static int h(const GridPos &pt1, const GridPos &pt2)
    {
        return abs(pt1.row() - pt2.row()) + abs(pt1.column() - pt2.column());
    }

private:
    Algorithm();
    Algorithm(const Algorithm &);
    Algorithm operator =(const Algorithm &);

    /*! Converts from the bi-dimensional matrix coordinate (N x N) to the uni-dimensional array coordinate (N^2).
      * @param pos bi-dimensional coordinate
      * @param n the dimension of the matrix (N x N)
      * @return The uni-dimensional coordinate
      */
    static int from2To1(const GridPos &pos, int n)
    {
        return pos.row() * n + pos.column();
    }

    /*! Converts from the uni-dimensional array coordinate (N^2) to the bi-dimensional matrix coordinate (N x N).
      * @param offset the uni-dimensional coordinate
      * @param the dimension of the matrix (N x N)
      * @return The bi-dimensional coordinate
      */
    static GridPos from1To2(int offset, int n)
    {
        if (0 == n) {
            return GridPos(-1, -1);
        }

        return GridPos(offset / n, offset % n);
    }
};

/*!
  * Implements the operator << for writing in the debug stream of Qt.
  */
inline QDebug operator <<(QDebug debug, const Algorithm::PathCost &pthc)
{
    debug.nospace() << pthc.m_gridPos.row() << ", " << pthc.m_gridPos.column() << ", (" << pthc.m_cost << ")";
    return debug.space();
}

#endif // ALGORITHM_HPP
