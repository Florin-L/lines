/*!
  * @file ballitemsprovider.hpp
  *
  * This file contains the declaration of the class BallItemsProvider.
  */

#ifndef BALLITEMSPROVIDER_HPP
#define BALLITEMSPROVIDER_HPP

#include <QtCore/QVector>
#include <QtGui/QColor>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QList>
#include "griditem.hpp"
#include "singleton.hpp"
#include "ballpaintinfo.hpp"

// forward declarations
class BallItem;

/*! This class handles the creation of the new ball items.
  *
  * It also manages the lists that store the available and the occupied positions in the grid.
  * This class implements the singleton pattern.
  */
class BallItemsProvider : public Singleton<BallItemsProvider>
{
public:
    /*! Sets the grid item.
      * @param[in] grid the grid item
      * \sa grid()
      */
    inline void setGrid(GridItem *grid)
    {
        m_grid = grid;
    }

    /*!
      * @return the grid item.
      * \sa setGrid()
      */
    inline GridItem* grid()
    {
        return m_grid;
    }

    /*! Gets and displays the set of the next three balls.
      * @param[in] enforceHints if true it forces the producing of the 'hint' balls
      */
    const QList<BallItem*>& nextBalls(bool enforceHints = false);

    /*!
      * @return the list of the hint balls.
      */
    inline QList<BallItem*>& hintBalls()
    {
        return m_hintBalls;
    }

    /*! Resets all the lists: the hints and current balls as well as the used and available positions.
      */
    void reset();

    /*! Initializes the lists of the ball items and the textures used to render the balls.
      * @param[in] grid the grid item
      */
    void init(GridItem *);

    /*! Creates a new ball and selects randomly the painting context (\sa m_colors).
      */
    BallItem *createBall();

    /*! Remove a ball item from the list of the hint ball items.
      * @param[in] ball the ball item to be removed from the list of the hint ball items.
      */
    void removeHint(BallItem *);

    /*!
      * @return the reference to the list of the available indexes
      * \sa usedIndexes()
      */
    inline QList<int>& availableIndexes()
    {
        return m_availableIdxs;
    }

    /*!
      * @return the reference to the list of the used indexes
      * \sa availableIndexes()
      */
    inline QList<int>& usedIndexes()
    {
        return m_usedIdxs;
    }

    /*! Transfer a index from the list of the available indexes into the list of the used ones.
      * @param[in] pos the coordinate of the cell that is to be occupied
      * \sa fromUsedToAvailable()
      */
    void fromAvailableToUsed(const GridPos &pos);

    /*! Transfer a index from the list of the used indexes into the list of the available ones.
      * @param[in] pos the coordinate of the cell that is to be freed
      * \sa fromAvailableToUsed
      */
    void fromUsedToAvailable(const GridPos &pos);

    /*! Transfer a index from the list of the used indexes into the list of the available ones.
      * @param[in] pos the coordinate of the cell that is to become available
      * \sa toUnidimensionalIndex(int roe, int col)
      */
    inline int toUnidimensionalIndex(const GridPos &pos)
    {
        return pos.row() * m_grid->dim() + pos.column();
    }

    /*! Convert a bi-dimensional index in a matrix to a uni-dimensional index.
      * @param[in] row the row
      * @param[in] col the column
      * \sa toUnidimensionalIndex(const GridPos &pos)
      */
    inline int toUnidimensionalIndex(int row, int col)
    {
        return row * m_grid->dim() + col;
    }

private:
    QVector<QSharedDataPointer<BallItemPaintCntx> > m_colors; /*!< the painting contexts for rendering the ball items */

    GridItem *m_grid; /*!< the grid */
    QList<BallItem*> m_hintBalls; /*!< the set of the 'hint' ball items */
    QList<BallItem*> m_currentBalls; /*! the set of the current ball items */

    QList<int> m_availableIdxs; /*!< the available indexes */
    QList<int> m_usedIdxs; /*!< the used indexes */
};

#endif // BALLITEMSPROVIDER_HPP
