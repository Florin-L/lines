/*!
  * @file linestracker.cpp
  * This file contains the definition of the class LinesTracker.
  */

#include "linestracker.hpp"
#include "ballitem.hpp"
#include "griditem.hpp"


/*!
  */
void LinesTracker::clear()
{
    for (int i = HORIZ; i <= DIAG2; ++i)
        m_lines[i].clear();
}

/*!
  */
void LinesTracker::search(GridItem *grid, BallItem *ball)
{
    Q_ASSERT(grid != 0);
    Q_ASSERT(ball != 0);

    clear();

    //
    GridPos ballPos = ball->coordinates();

    // Horizontal line
    // W-E direction
    GridPos pos = ballPos.left();

    m_lines[HORIZ].push_back(ballPos);
    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[HORIZ].push_front(pos);
        pos = pos.left();
    }

    pos = ballPos.right();
    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[HORIZ].push_back(pos);
        pos = pos.right();
    }

    // Vertical line
    // N-S direction
    m_lines[VERT].push_back(ballPos);
    pos = ballPos.up();

    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[VERT].push_front(pos);
        pos = pos.up();
    }

    pos = ballPos.down();
    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[VERT].push_back(pos);
        pos = pos.down();
    }

    // NW-SE direction
    m_lines[DIAG1].push_back(ballPos);
    pos = ballPos.left().up();

    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[DIAG1].push_front(pos);
        pos = pos.left().up();
    }

    pos = ballPos.right().down();
    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[DIAG1].push_front(pos);
        pos = pos.right().down();
    }

    // SW-NE direction
    m_lines[DIAG2].push_back(ballPos);
    pos = ballPos.left().down();

    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[DIAG2].push_front(pos);
        pos = pos.left().down();
    }

    pos = ballPos.right().up();
    while (grid->isValidPosition(pos) &&
           !grid->isFreePos(pos) && (grid->ballAt(pos)->color() == ball->color())) {
        m_lines[DIAG2].push_front(pos);
        pos = pos.right().up();
    }
}
