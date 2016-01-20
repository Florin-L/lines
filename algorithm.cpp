/*!
  * @file algorithm.cpp
  */

#include <QtCore/QList>
#include "griditem.hpp"
#include "algorithm.hpp"

/*!
  *
  */
bool Algorithm::shortestPath(GridItem *grid, GridPos &beginPos, GridPos &endPos, QList<GridPos> &path)
{
    static int f[GridItem::dimension][GridItem::dimension]; // the cost function : f = g + h
    static int g[GridItem::dimension][GridItem::dimension]; //
    static int cameFrom[GridItem::dimension * GridItem::dimension]; //

    // initialization of the data structures
    for (int i = 0; i < GridItem::dimension; ++i) {
        memset((void*)&f[i], 0, sizeof(int) * GridItem::dimension);

        for (int j = 0; j < GridItem::dimension; ++j)
            g[i][j] = 9999;
    }

    int n = GridItem::dimension * GridItem::dimension;
    for (int i = 0; i < n; ++i)
        cameFrom[i] = -1; // initializes with -1 because 0 is the index of the node (0, 0) !

    path.clear();
    //

    qDebug() << "find the shortest path between " << beginPos << " and " << endPos;

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
            reconstructPath(cameFrom, from2To1(endPos, GridItem::dimension), path);
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
                cameFrom[from2To1(pt1, GridItem::dimension)] = from2To1(pt, GridItem::dimension);

                g[pt1.row()][pt1.column()] = gg;
                f[pt1.row()][pt1.column()] = gg + h(pt, pt1);
            }
        }
        neighbours.clear();
    }

    return false;
}

//
void Algorithm::reconstructPath(int *cameFrom, int currentNode, QList<GridPos> &path)
{
    while (cameFrom[currentNode] != -1) {
        path.push_front(from1To2(currentNode, GridItem::dimension));
        currentNode = cameFrom[currentNode];
    }
}
