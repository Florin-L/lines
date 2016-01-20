/*!
  * @file pathfinder.cpp
  * This file contains the definition of the class PathFinder.
  */

#include <QtCore/QVector>
#include "griditem.hpp"
#include "pathfinder.hpp"

template<> PathFinder* Singleton<PathFinder>::s_instance = 0;

/*!
  */
PathFinder::~PathFinder()
{
    destroyArrays();
}

/*!
  */
void PathFinder::init(int dimension)
{
    // initializes the data structures
    m_dimension = dimension;

    f = new PUINT16[dimension];
    for (int i = 0; i < dimension; ++i) {
        f[i] = new UINT16[dimension];
        memset(reinterpret_cast<void*>(f[i]), 0, sizeof(UINT16) * m_dimension);
    }

    g = new PUINT16[dimension];
    for (int i = 0; i < dimension; ++i) {
        g[i] = new UINT16[dimension];
        memset(reinterpret_cast<void*>(g[i]), 0xFFFF, sizeof(UINT16) * m_dimension); // a resonable big value (65535)
    }

    int n = dimension * dimension;
    cameFrom = new INT16[n];
    for (int i = 0; i < n; ++i) {
        cameFrom[i] = -1; // initializes with -1 because 0 is the index of the node (0, 0) !
    }
}

/*!
  */
void PathFinder::destroyArrays()
{
    delete[] cameFrom;

    for (int i = 0; i < m_dimension; ++i) {
        delete[] f[i];
    }
    delete[] f;

    for (int i = 0; i < m_dimension; ++i) {
        delete[] g[i];
    }
    delete[] g;
}

/*!
  */
void PathFinder::clearArrays()
{
    for (int i = 0; i < m_dimension; ++i) {
        memset(reinterpret_cast<void*>(f[i]), 0, sizeof(UINT16) * m_dimension);
    }

    for (int i = 0; i < m_dimension; ++i) {
        memset(reinterpret_cast<void*>(g[i]), 0xFFFF, sizeof(UINT16) * m_dimension);
    }

    int n = m_dimension * m_dimension;
    for (int i = 0; i < n; ++i) {
        cameFrom[i] = -1; // initializes with -1 because 0 is the index of the node (0, 0) !
    }
}

/*!
  */
bool PathFinder::execute(GridItem *grid, GridPos &beginPos, GridPos &endPos, QVector<GridPos> &path)
{
    if (path.count()) {
        path.clear();
    }

    clearArrays();

    OpenSet_t openList;
    QList<GridPos> closedList;

    //
    openList.insert(PathCost(beginPos, h(beginPos, endPos)));

    while (!openList.empty()) {
        // Retrieves the node from the openset having the lowest f[][] value.
        OpenSetIter_t ib = openList.begin();
        GridPos pt = ib->m_gridPos;

        // Also removes this node from the openset.
        openList.erase(ib);

        if (pt == endPos) {
            reconstructPath(cameFrom, from2To1(endPos, grid->dim()), path, grid->dim());
            path.push_front(beginPos);
            return true;
        }

        // Add the current square into the closedset.
        closedList.append(pt);

        // Get the available neighbours of the current square.
        QList<GridPos> neighbours;
        if ((pt.column() > 0) && grid->isFreePos(pt.left())) {
            neighbours.append(pt.left());
        }

        if ((pt.row() > 0) && grid->isFreePos(pt.up())) {
            neighbours.append(pt.up());
        }

        if ((pt.column() < grid->dim() - 1) && grid->isFreePos(pt.right())) {
            neighbours.append(GridPos(pt.right()));
        }

        if ((pt.row() < grid->dim() - 1) && grid->isFreePos(pt.down())) {
            neighbours.append(GridPos(pt.down()));
        }

        int n = neighbours.size();
        for (int i = 0; i < n; ++i) {
            //
            GridPos pt1 = neighbours[i];
            if (closedList.contains(pt1)) {
                continue;
            }

            // the cost of moving from the square 'pt' to the square 'pt1' is always incremented by 1.
            int gg = g[pt.row()][pt.column()] + 1;

            PathCost pthc_pt1(pt1);

            int offset = findElementInSet(openList, pthc_pt1.m_gridPos);

            OpenSetIter_t liter = openList.begin();
            if (offset >= 0) {
                std::advance(liter, offset);
            }

            bool betterAlternative = false;
            if (offset == -1) {
                // the neighbour is not in the openset yet: add the neighbour into the openset.
                openList.insert(pthc_pt1);
                betterAlternative = true;
            } else if (gg < liter->m_cost) {
                // reinsert the neighbour into the openset with the updated cost.
                pthc_pt1.m_cost = gg;

                openList.erase(liter);
                openList.insert(pthc_pt1);

                betterAlternative = true;
            }

            if (betterAlternative) {
                cameFrom[from2To1(pt1, grid->dim())] = from2To1(pt, grid->dim());

                g[pt1.row()][pt1.column()] = gg;
                f[pt1.row()][pt1.column()] = gg + h(pt, pt1);
            }
        }
        neighbours.clear();
    }

    return false;
}

/*!
  */
int PathFinder::findElementInSet(const OpenSet_t &list,
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

/*!
  */
void PathFinder::reconstructPath(PINT16 cameFrom, int currentNode, QVector<GridPos> &path, int dimension)
{
    while (cameFrom[currentNode] != -1) {
        path.push_front(from1To2(currentNode, dimension));
        currentNode = cameFrom[currentNode];
    }
}
