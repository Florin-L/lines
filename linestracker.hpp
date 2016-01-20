/*!
  * @file linestracker.hpp
  * This file contains the declaration of the class LinesTracker.
  */
#ifndef LINESTRACKER_HPP
#define LINESTRACKER_HPP

#include <QtCore/QList>
#include <QtCore/QSet>
#include "gridpos.hpp"
#include "singleton.hpp"

// forward declarations
class BallItem;
class GridItem;

/*! This class implements a serching method that looks for all the rows of the ball items
  * that have the same color. This class is a singleton.
  */
class LinesTracker : public Singleton<LinesTracker>
{
public:
    /*!
      * The directions the 'tracker' moves on.
      */
    enum
    {
        HORIZ, // W-E direction
        VERT,  // N-S direction
        DIAG1, // NW-SE direction
        DIAG2  // SW-NE direction
    };

    /*! The destructor
      */
    inline ~LinesTracker()
    {
        clear();
    }

    /*! Searches all the four directions starting with the position of the given ball item for
      * rows of ball items having the same color.
      *
      * @param[in] grid the grid item
      * @param[in] ball the ball item where the searching starts from
      */
    void search(GridItem *grid, BallItem *);

    /*! cleans up the internal structure
      */
    void clear();

    /*!
      * @return the horizontal positions
      *
      * \sa verticalLine(), diag1Line(), diag2Line()
      */
    inline const QList<GridPos>& horizontalLine() const
    {
        return m_lines[HORIZ];
    }

    /*!
      * @return the vertical positions
      *
      * \sa horizontalLine(), diag1Line(), diag2Line()
      */
    inline const QList<GridPos>& verticalLine() const
    {
        return m_lines[VERT];
    }

    /*!
      * @return the positions along the NW-SE direction
      *
      * \sa verticalLine(), verticalLine(), diag2Line()
      */
    inline const QList<GridPos>& diag1Line() const
    {
        return m_lines[DIAG1];
    }

    /*!
      * @return the positions along the SW-NE direction
      *
      * \sa verticalLine(), horizontalLine(), diag1Line()
      */
    inline const QList<GridPos>& diag2Line() const
    {
        return m_lines[DIAG2];
    }

    //
    typedef QList<GridPos> list_of_positions_t;
    typedef QSet<GridPos> set_of_positions_t;
    typedef list_of_positions_t (&lines_t)[4];
    typedef const list_of_positions_t (&const_lines_t)[4];

    /*!
      * @return the list of the lines
      */
    const_lines_t lines()
    {
        return m_lines;
    }

private:
    QList<GridPos> m_lines[4]; /*!< the list of the lines */
};

#endif // LINESTRACKER_HPP
