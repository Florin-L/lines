/*!
  * @file pathfinder.hpp
  * This file contains the declaration of the class PathFinder.
  */
#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <cmath>
#include <set>
#include "gridpos.hpp"
#include "singleton.hpp"

// forward declarations
class GridItem;

/*! \brief This class encapsulates the implementation of the A* algorithm for finding the path
  * between two squares in the grid. 
  *
  * This class is a singleton.
  */
class PathFinder : public Singleton<PathFinder>
{
    typedef quint16 UINT16;
    typedef UINT16* PUINT16;
    typedef qint16  INT16;
    typedef INT16*  PINT16;

public:
    /*! \brief Wraps the coordinates and the cost of a square.
      *
      */
    struct PathCost
    {
        /*! The constructor.
          *
          * @param[in] gridPos the position in grid
          * @param[in] cost the cost of the path through this position
          */
        PathCost(const GridPos &gridPos, int cost = 0)
                : m_gridPos(gridPos), m_cost(cost)
        {
        }

        GridPos m_gridPos; /*!< the position */
        int m_cost; /*!< the cost */
    };

    /*! \brief The functor used to sort the elements of the openlist.
      *
      */
    struct CmpCostPath
    {
        /*! The function operator.
          *
          * Compares the costs for two positions in the grid.
          *
          * @param[in] pthc1 the first position
          * @param[in] pthc2 the second position
          */
        bool operator()(const PathCost &pthc1, const PathCost &pthc2)
        {
            return pthc1.m_cost < pthc2.m_cost;
        }
    };

    /*! The destructor.
      */
    ~PathFinder();

    /*! Initializes the cost and the path arrays.
      *
      */
    void init(int dimension);

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
      * @param[in] list The openlist.
      * @param[in] coords The coordinates of the square.
      * @return If the square is found then its position in openlist is returned; otherwise returns -1.
      */
    int findElementInSet(const OpenSet_t &list, const GridPos &coords);

    void dumpOpenSet(OpenSet_t list)
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

    /*! Implementation of the A* algorithm to find the shortest path between the starting and the ending positions in grid.
      *
      * @param[in] grid     The grid we searche the path within.
      * @param[in] beginPos The starting position on the grid.
      * @param[in] endPos   The ending position on the grid.
      * @param[out] path     The shortest path between the given coordinates.
      */
    bool execute(GridItem *, GridPos &, GridPos &, QVector<GridPos> &);

    /*! Reconstructs the path between two positions in the grid.
      * The positions are expressed as uni-dimensional indexes.
      * 
      * @param[in] cameFrom the starting position
      * @param[in] currentNode the current positions
      * @param[out] path the path so far
      * @param[in] dimension the dimension of the grid
      */
    void reconstructPath(PINT16, int, QVector<GridPos> &, int);

    /*! Computes the Manhattan estimation between two squares of the grid.
      *
      * @param[in] pt1 the first position
      * @param[in] pt2 the second position
      * @return the Manhattan distance betwen the position pt1 and position pt2.
      */
    int h(const GridPos &pt1, const GridPos &pt2)
    {
        return abs(pt1.row() - pt2.row()) + abs(pt1.column() - pt2.column());
    }

private:
    /*!
      */
    void destroyArrays();

    /*!
      */
    void clearArrays();

    /*! Converts from the bi-dimensional matrix coordinate (N x N) to the uni-dimensional array coordinate (N^2).
      * @param[in] pos bi-dimensional coordinate
      * @param[in] n the dimension of the matrix (N x N)
      * @return The uni-dimensional coordinate
      */
    INT16 from2To1(const GridPos &pos, int n)
    {
        return INT16(pos.row() * n + pos.column());
    }

    /*! Converts from the uni-dimensional array coordinate (N^2) to the bi-dimensional matrix coordinate (N x N).
      * @param[in] offset the uni-dimensional coordinate
      * @param[in] the dimension of the matrix (N x N)
      * @return The bi-dimensional coordinate
      */
    GridPos from1To2(INT16 offset, int n)
    {
        if (0 == n) {
            return GridPos(-1, -1);
        }

        return GridPos(offset / n, offset % n);
    }

private:
    PUINT16 *f; // the matrix that keeps the overall cost function : f = g + h
    PUINT16 *g; // the matrix that keeps the cost of the path from the starting position to the current one.
    PINT16 cameFrom; // the array that stores the path between two positions
    int m_dimension;
};

/*!
  * Implements the operator << for writing in the debug stream of Qt.
  */
inline QDebug operator <<(QDebug debug, const PathFinder::PathCost &pthc)
{
    debug.nospace() << pthc.m_gridPos.row() << ", " << pthc.m_gridPos.column() << ", (" << pthc.m_cost << ")";
    return debug.space();
}

#endif // PATHFINDER_HPP
