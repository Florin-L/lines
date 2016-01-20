/*!
  * @file gridpos.hpp
  * This file contains the declaration of the class GridPos.
  */

#ifndef GRIDPOS_HPP
#define GRIDPOS_HPP

#include <QtCore/QDebug>

// hash function for the instances of the type GridPos
class GridPos;

/*! Computes the hash value for an instance of the class GridPos.
  * This function is required by the operation upon a hash set data structure.
  *
  * @param[in] pos an instance of the class GridPos
  * @return the computed hash value.
  */
uint qHash(const GridPos &pos);

/*! This class implements a position in grid matrix coordinates.
  */
class GridPos
{
public:
    /*! The default constructor.
      * \sa GridPos(int, int), GridPos(const GridPos&)
      */
    GridPos() : m_row(0), m_column(0)
    {
    }

    /*! The constructor.
      * @param[in] row
      * @param[in] column
      *
      * \sa GridPos(), GridPos(const GridPos&)
      */
    GridPos(int row, int column) : m_row(row), m_column(column)
    {
    }

    /*! The copy construcor.
      * @param[in] pos
      *
      * \sa GridPos(), GridPos(int, int)
      */
    GridPos(const GridPos &pos) : m_row(pos.m_row), m_column(pos.m_column)
    {
    }

    /*! The assignement operator.
      * @param[in] pos
      */
    inline GridPos operator =(const GridPos &pos)
                             {
        if (this == &pos) {
            return *this;
        }

        m_row = pos.m_row;
        m_column = pos.m_column;
        return *this;
    }

    /*! The equal operator.
      * @param[in] pos
      */
    inline bool operator ==(const GridPos &pos) const
    {
        if (this == &pos) {
            return true;
        }

        return (m_column == pos.m_column) && (m_row == pos.m_row);
    }

    /*!
      * @param[in] pos
      */
    inline bool operator !=(const GridPos &pos) const
    {
        return (m_column != pos.m_column) || (m_row != pos.m_row);
    }

    /*! The getter for the attribute row.
      * @return The row of the grid position.
      *
      * \sa setRow(), column()
      */
    inline int row() const
    {
        return m_row;
    }

    /*! The getter for the attribute column.
      * @return The column of the grid position.
      *
      * \sa setColumn(), row()
      */
    inline int column() const
    {
        return m_column;
    }

    /*! The setter for the attribute row.
      * @param[in] row the row coordinate
      *
      * \sa row(), setColumn()
      */
    inline void setRow(int row)
    {
        m_row = row;
    }

    /*! The setter for the attribute column.
      * @param[in] column the column coordinate
      *
      * \sa column(), setRow()
      */
    inline void setColumn(int column)
    {
        m_column = column;
    }

    /*!
      * @return the position to the left of this position
      *
      * \sa right(), up(), down()
      */
    inline GridPos left()
    {
        return GridPos(row(), column()-1);
    }

    /*!
      * @return the position to the right of this position
      *
      * \sa left(), up(), down()
      */
    inline GridPos right()
    {
        return GridPos(row(), column()+1);
    }

    /*!
      * @return the position above this position
      *
      * \sa left(), right(), down()
      */
    inline GridPos up()
    {
        return GridPos(row()-1, column());
    }

    /*!
      * @return the position below this position
      *
      * \sa left(), right(), up()
      */
    inline GridPos down()
    {
        return GridPos(row()+1, column());
    }

private:
    int m_row; /*!< The attribute row. */
    int m_column; /*!< The attribute column. */
};

/*! Implements the operator "<<" in order to append the instances of the type GridPos
  * to a debug stream.
  *
  * @param[in] debug the debug stream
  * @param[in] pos the instance of the type GridPos
  * @return the debug stream
  */
inline QDebug operator <<(QDebug debug, const GridPos &pos)
{
    debug.nospace() << "GridPos(";
    debug << pos.row() << ", " << pos.column() << ")";

    return debug.space();
}

#endif // GRIDPOS_HPP
