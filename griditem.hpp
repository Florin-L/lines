/*!
* @file griditem.hpp
* This file contains the declaration of the class GridItem.
*/
#ifndef GRIDITEM_HPP
#define GRIDITEM_HPP

#include <QGraphicsItem>
#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QDebug>
#include "gridpos.hpp"
#include "ballitem.hpp"
#include "pathtracker.hpp"
#include "linestracker.hpp"

// forward declarations
class QGraphicsSceneMouseEvent;

/*!
* The left upper cell of the grid has the coordinate (0, 0) while the coordinate of
* the right bottom cell is (\a m_dimension - 1, \a m_dimension - 1).
*
* \par -----------------------------
* \par| (0,0) | (0,1) |...| (0,n-1) |
* \par -----------------------------
* \par| (1,0) | (1,1) |...|( 1,n-1) |
* \par -----------------------------
* \par.
* \par.
* \par.
* \par -----------------------------------
* \par| (n-1,0) | (n-1,1) |...| (n-1,n-1) |
*
* For the board view the x-coordinates increase from left to right while the y-coordinates
* increase from top to bottom.
*/

/*! This class implements the grid item.
  */
class GridItem : public QGraphicsItem
{
public:
    /*! The constructor.
      * @param[in] dimension the dimension of the grid
      */
    GridItem(int dimension);

    /*! The destructor.
      */
    ~GridItem();

    //! overrided methods

    /*! Returns the bounding rectangle of this item.
      * @return the bounding item
      */
    QRectF boundingRect() const;

    /*! Renders the grid item.
      * @param[in] painter the painter
      * @param[in] options the parameters used to paint this graphic item
      * @param[in] widget the widget that is painted on
      */
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    /*! Shows a ball at a given position in grid.
    *
    * @param[in] ball The ball to be shown.
    * @param[in] row the row
    * @param[in] col the column
    * @param[in] updateInternalStruct
    *
    * \sa showBall(BallItem*, const GridPos&, bool),
    * showBall(BallItem*), hideBall(BallItem*)
    */
    void showBall(BallItem *, int row, int col, bool updateInternalStruct = true);

    /*! Shows a ball at a given position in grid.
      *
      * @param[in] ball the ball to be shown
      * @param[in] pos the position where the ball is to be shown
      * @param[in] updateInternalStruct
      *
      * \sa showBall(BallItem*, int, int, bool), showBall(BallItem*), hideBall(BallItem*)
      */
    void showBall(BallItem *, const GridPos &, bool updateInternalStruct = true);

    /*! Shows a ball in grid. The position of the ball is read from the BallItem's property m_coord.
      *
      * @param[in] ball The ball to be shown.
      *
      * \sa showBall(BallItem*, int, int, bool),
      * showBall(BallItem*, const GridPos&, bool), hideBall(BallItem*)
      */
    void showBall(BallItem *);

    /*! Hides the ball at the given position in grid.
      *
      * @param[in] row the row
      * @param[in] col the column
      * @return A pointer to the item that was hidden (the NULL pointer if there was any ball to be hidden).
      *
      * \sa hideBall(const GridPos&), hideBall(BallItem*), showBall(BallItem*)
      */
    inline BallItem* hideBall(int row, int col)
    {
        Q_ASSERT(isValidPosition(row, col));
        return hideBall(m_balls[row][col]);
    }

    /*! Hides the ball at the given position in grid.
      *
      * @param[in] pos the position of the ball item
      * @return A pointer to the item that was hidden (the NULL pointer if there was any ball to be hidden).
      *
      * \sa hideBall(int, int), hideBall(BallItem*), showBall(BallItem*)
      */
    inline BallItem* hideBall(const GridPos &pos)
    {
        return hideBall(pos.row(), pos.column());
    }

    /*! Hides the given ball. The position of the ball is read from the BallItem's property m_coord.
      *
      * @param[in] ball the ball to be hidden
      * @return A pointer to the item that was hidden (the NULL pointer if there was any ball to be hidden).
      *
      * \sa hideBall(int, int), hideBall(const GridPos&), showBall(BallItem*)
      */
    BallItem* hideBall(BallItem *ball);

    /*! Moves a ball along a path that consists of squares of the grid.
    *
    * @param[in] ball the ball to be moved on
    * @param[in] path the list of the squares that form the path where the ball is to be moved on
    */
    void moveBall(BallItem *ball, const QVector<GridPos> &path);

    /*! Checks out whether a given position in grid is available.
    *
    * @param[in] row the row
    * @param[in] col the column
    * @return true if the cell is available, false otherwise
    *
    * \sa isEmptyPos(const GridPos&), isFreePos(int, int), isFreePos(const GridPos&),
    * freePos(int, int), freePos(const GridPos&)
    */
    inline bool isEmptyPos(int row, int col)
    {
        return m_balls[row][col] == 0;
    }

    /*! Checks out whether a given position in grid is available.
      *
      * @param[in] pos the position in the grid coordinates
      * @return true if the cell is available, false otherwise
      *
      * \sa isEmptyPos(int, int), isFreePos(int, int), isFreePos(const GridPos&),
      * freePos(int, int), freePos(const GridPos&)
      */
    inline bool isEmptyPos(const GridPos &pos)
    {
        return isEmptyPos(pos.row(), pos.column());
    }

    /*! Checks whether a square contains a ball item of any kind.
      *
      * @param[in] row
      * @param[in] col
      * @return true if there is no ball item of any kind at the given position, false otherwise
      *
      * \sa isFreePos(const GridPos&), freePos(int, int), freePos(const GridPos&),
      * isEmptyPos(int, int), isEmptyPos(const GridPos&)
      */
    inline bool isFreePos(int row, int col)
    {
        return isEmptyPos(row, col) || isHintPos(row, col);
    }

    /*! Checks whether a square contains a ball item of any kind.
      *
      * @param[in] pos the position of the square
      * @return true if there is no ball item of any kind at the given position, false otherwise
      *
      * \sa isFreePos(int, int), freePos(int, int), freePos(const GridPos&),
      * isEmptyPos(int, int), isEmptyPos(const GridPos&)
      */
    inline bool isFreePos(const GridPos &pos)
    {
        return isFreePos(pos.row(), pos.column());
    }

    /*! Checks whether a square contains a hint ball item.
      *
      * @param[in] row the row
      * @param[in] col the column
      * @return true if the ball item painted on the square at the given coordinates is a 'hint ball'; false otherwise
      *
      * \sa isHintPos(const GridPos&)
      */
    inline bool isHintPos(int row, int col)
    {
        return m_balls[row][col]->isHint();
    }

    /*! Checks whether a square contains a hint ball item.
      *
      * @param[in] pos the position
      * @return true if the ball item painted on the square at the given coordinates is a 'hint ball'; false otherwise
      *
      * \sa isHintPos)int, int)
      */
    inline bool isHintPos(const GridPos &pos)
    {
        return isHintPos(pos.row(), pos.column());
    }

    /*! Gets the ball item that is rendered at a given position in the grid.
      *
      * @param[in] pos the position in the grid coordinates.
      * @return the ball item that is rendered at these coordinates if any; null pointer otherwise.
      *
      * \sa ballAt(int, int), setBallAt(const GridPos&, BallItem*), setBallAt(int, int, BallItem*)
      */
    inline BallItem* ballAt(const GridPos &pos)
    {
        return ballAt(pos.row(), pos.column());
    }

    /*! Gets the ball item that is rendered at a given position in the grid.
      *
      * @param[in] row the row of the cell.
      * @param[in] col the column of the cell.
      * @return A pointer to the ball item that is rendered at these coordinates if any; null pointer otherwise
      *
      * \sa ballAt(const GridPos &), setBallAt(const GridPos&, BallItem*), setBallAt(int, int, BallItem*)
      */
    inline BallItem* ballAt(int row, int col)
    {
        Q_ASSERT(isValidPosition(row, col));
        return m_balls[row][col];
    }

    /*!
      * @param[in] pos the position
      * @param ball the ball to be set at
      *
      * \sa setBallAt(int, int, BallItem*), ballAt(const GridPos&), ballAt(int, int)
      */
    inline void setBallAt(const GridPos& pos, BallItem *ball)
    {
        setBallAt(pos.row(), pos.column(), ball);
    }

    /*!
      * @param[in] row the row
      * @param[in] col the column
      * @param[in] ball the ball item
      *
      * \sa setBallAt(const GridPos&, BallItem*), ballAt(const GridPos&), ballAt(int, int)
      */
    inline void setBallAt(int row, int col, BallItem *ball)
    {
        Q_ASSERT(isValidPosition(row, col));
        m_balls[row][col] = ball;
    }

    /*!
    * Reset the grid.
    */
    void reset();

    /*! Marks a cell as being available in the internal structure of the grid. The method only sets the pointer at (row, col)
      * to NULL but does not free up the ball item that might be assigned to the given coordinate.
      *
      * @param[in] row
      * @param[in] col
      *
      * \sa freePos(const GridPos &), isFreePos(const GridPos&), isFreePos(int, int)
      */
    inline void freePos(int row, int col)
    {
        Q_ASSERT(isValidPosition(row, col));
        m_balls[row][col] = 0;
    }

    /*! Marks a cell as being available in the internal structure of the grid. The method only sets the pointer at (row, col)
      * to NULL but does not free up the ball item that might be assigned to the given coordinate.
      *
      * @param[in] pos the position of a square
      *
      * \sa freePos(int, int), isFreePos(const GridPos&), isFreePos(int, int)
      */
    inline void freePos(const GridPos &pos)
    {
        freePos(pos.row(), pos.column());
    }

    /*! Selects or unselects a ball at a given position on the board.
      * The diameter of a selected ball is slightly greater than of a normal one.
      *
      * @param[in] row the row
      * @param[in] column the column
      * @param[in] selectFlag If it is true then the ball is to be rendered as selected. Otherwise the ball
      * is rendered with a normal diameter.
      *
      * \sa selectBall(const GridPos &, bool)
      */
    inline void selectBall(int row, int column, bool selectFlag = true)
    {
        BallItem *ball = ballAt(row, column);
        if (ball) {
            ball->select(selectFlag, true); // also repaint the ball to reflect the selection
        }
    }

    /*! Selects or unselects a ball at a given position on the board.
      * The diameter of a selected ball is slightly greater than of a normal one.
      *
      * @param[in] pos The position of the ball in the grid coordinates.
      * @param[in] selectFlag If it is true then the ball is to be rendered as selected. Otherwise the ball
      * is rendered with a normal diameter.
      *
      * \sa selectBall(int, int, bool)
      */
    inline void selectBall(const GridPos &pos, bool selectFlag = true)
    {
        selectBall(pos.row(), pos.column(), selectFlag);
    }
    
    /*! Removes the lines of the balls from grid.
      * @param[in] positions the set of the positions that are to freed up.
      * @return The score corresponding to the removed balls.
      */
    int removeLines(LinesTracker::set_of_positions_t &positions);

    /*! Animates the balls that are to be removed from grid.
      *
      * @param[in] positions the set of the positions that are to be animated
      */
    void animateBalls(LinesTracker::set_of_positions_t &positions);

    /*! Gets the dimension (rows x columns) of the grid.
      * @return The dimension of the grid.
      */
    inline int dim() const
    {
        return m_dimension;
    }

    /*! Checks whether the coordinates of a given position do not exceed the borders of the grid.
      *
      * @param[in] pos the position to be checked.
      *
      * \sa isValidPosition(int, int)
      */
    inline bool isValidPosition(const GridPos &pos)
    {
        return isValidPosition(pos.row(), pos.column());
    }

    /*! Checks whether the coordinates of a given position do not exceed the borders of the grid.
      *
      * @param[in] row the row
      * @param[in] column the column
      *
      * \sa isValidPosition(const GridPos&)
      */
    inline bool isValidPosition(int row, int column)
    {
        return (row >= 0) && (row < dim()) && (column >= 0) && (column < dim());
    }

    /*!
      * @return the total number of the squares in grid
      */
    inline int size() const
    {
        return m_size;
    }

    /*! Searches the all directions for rows of balls of the same color.
      *
      * @param[in] ball the ball that was added on the grid
      *
      * \sa checkLines(const QList<BallItem*>&)
      */
    void checkLines(BallItem *ball);

    /*! Searches the all directions for rows of balls of the same color.
      *
      * @param[in] balls the balls that were added on the grid
      *
      * \sa checkLines(BallItem*)
      */
    void checkLines(const QList<BallItem*> &balls);

    /*! The reseting game animation effect : animate all the balls on the grid.
      */
    void resetAnimation();

    /*! Handles the mouse press event.
      * @param[in] event The mouse event.
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *);

    /*! Handles the mouse release event.
      * @param[in] event the mouse event
      */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

    /*! Handles the mouse move event.
      * @param[in] event the mouse event
      */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);

    /*! Prints out the matrix of the ball items.
      */
    void dumpBallsMatrix();

protected:

    /*! Maps the a given (row, column) coordinate to the center of a grid cell.
    * @param[in] row
    * @param[in] col
    * @param[out] gridPos the position of the center of a square in pixels relative
    * to the left upper coordinate of the grid.
    *
    * \sa fromGridToCenteredCoordinate(const GridPos &, QPoint &)
    */
    void fromGridToCenteredCoordinate(int row, int col, QPoint &gridPos);

    /*! Maps the a given (row, column) coordinate to the center of a grid cell.
      *
      * @param[in] pos the position in grid coordinates
      * @param[out] centerPos the position of the center of the grid cell
      *
      * \sa fromGridToCenteredCoordinate(int, int, QPoint &)
      */
    void fromGridToCenteredCoordinate(const GridPos &pos, QPoint &centerPos);

    /*!
    *
    * @param[in] x The column's coordinate (X) in view's coordinates.
    * @param[in] y The row's coordinate (Y) in view's coordinates.
    * @param[out] gridCoord
    *
    * \sa fromViewToGridCoordinate(const QPoint &, GridPos &)
    */
    void fromViewToGridCoordinate(int x, int y, GridPos &gridCoord);

    /*!
    * @param[in] viewCoord The view's coordinates.
    * The property 'x' holds the column coordinate whereas the row coordinate is stored
    * in the property 'y' of an instance of the class QPoint.
    * @param[out] gridCoord
    *
    * \sa fromViewToGridCoordinate(int, int, GridPos &)
    */
    void fromViewToGridCoordinate(const QPoint &viewCoord, GridPos &gridCoord);

    /*!
    * @param[in] viewCoord The view's coordinates as floating point numbers.
    * @param[out] gridCoord
    *
    * \sa fromViewToGridCoordinate(int, int, GridPos &),
    * fromViewToGridCoordinate(const QPoint &, GridPos &)
    */
    void fromViewToGridCoordinate(const QPointF &viewCoord, GridPos &gridCoord);

    /*!
      * @param[in] ms
      * @param[in] ignoreUserEvents
      */
    void pause(int ms, bool ignoreUserEvents = true);

    /*! Inserts a grid positions into a set of positions where all ball items have the same color.
      * @param[in] set the existing set of the grid positions
      * @param[in] line the positions to be inserted in set
      */
    void insertIntoPositionSet(LinesTracker::set_of_positions_t &set, const LinesTracker::list_of_positions_t &line);

    /*! Prompts for ending or reseting the game.
      */
    int promptForGameEnd();

private:
    int m_dimension; /*!< the dimension of the grid */
    int m_penWidth; /*!< the width of the pen */

    BallItem *m_balls[9][9]; /*!< the matrix that stores the balls on the grid; the dimensions of the matrix are fixed */

    int m_squareSize; /*!< the size of a square on the grid; this value is fixed */
    GridPos m_beginPos; /*!< the initial position (in the grid coordinates) of the ball to be moved */
    GridPos m_endPos; /*!< the final position (in the grid coordinates) of the ball to be moved */
    bool m_ballSelected; /*!< did we select a ball ? */

    //int m_availabeCount; /*!< the number of the available positions on the grid */
    int m_size; /*!< the total number of positions in grid: dim() * dim() */
    PathTracker m_pathTracker; /*!< holds the path between two squares in grid */
};

#endif // GRIDITEM_HPP
