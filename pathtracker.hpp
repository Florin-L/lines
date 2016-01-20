/*!
  * @file pathtracker.hpp
  * This file contains the declaration of the class PathTracker.
  */
#ifndef PATHTRACKER_HPP
#define PATHTRACKER_HPP

#include <QtCore/QVector>
#include <QtCore/QPoint>
#include "gridpos.hpp"

/*! This class maintains two lists: a list (m_path) that stores the path between two squares of the grid
  * and another list (m_points) that keeps the pairs of the centers of the squares that forms the path
  * between two squares. The latter is used to draw the lines that connect the centers of the squares
  * in order to have the path drawn onto the grid.
  */
class PathTracker
{
public:
    /*! The constructor.
      */
    inline PathTracker() : m_points(), m_path()
    {}

    /*! The destructor.
      */
    inline ~PathTracker()
    {
        clear();
    }

    /*! Adds a point to this tracker.
      *
      * @param pt the point to be added
      *
      * \sa addLine(const QPoint &, const QPoint &)
      */
    inline void addPoint(const QPoint &pt)
    {
        m_points.push_back(pt);
    }

    /*! Adds a pair of points that form a line to this tracker.
      *
      * @param pt1
      * @param pt2
      *
      * \sa addPoint(const QPoint&)
      */
    inline void addLine(const QPoint &pt1, const QPoint &pt2)
    {
        m_points.push_back(pt1);
        m_points.push_back(pt2);
    }

    /*! Removes the pair of points from the front of the tracker.
      *
      * @return true if there was any pair of points in tracker, false otherwise
      */
    inline bool removeFrontLine()
    {
        bool retv = false;

        if (m_points.count() >= 2) {
            m_points.pop_front();
            m_points.pop_front();
            retv = true;
        }
        return retv;
    }

    /*! Cleans up the internal structure of the tracker.
      */
    inline void clear()
    {
        m_points.clear();
        m_path.clear();
    }

    /*!
      * @return all the points
      */
    QVector<QPoint>& points()
    {
        return m_points;
    }

    /*!
      * @return the grid positions that form a path between two squares in the grid
      */
    QVector<GridPos>& path()
    {
        return m_path;
    }

private:
    QVector<QPoint> m_points; /*!< the points */
    QVector<GridPos> m_path; /*!< the path between two positions in the grid */
};

#endif // PATHTRACKER_HPP
