/*!
  * @file ballitem.hpp
  *
  * This file contains the declaration of the class BallItem.
  */

#ifndef BALLITEM_HPP
#define BALLITEM_HPP

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QRadialGradient>
#include <QtCore/QSharedDataPointer>
#include "gridpos.hpp"
#include "ballpaintinfo.hpp"

/*! This class implements a ball item.
  */
class BallItem : public QGraphicsEllipseItem
{
public:
    /*! The constructor.
      * @param[in] parent the parent of this ball item
      */
    BallItem(QGraphicsItem * = 0);

    //! overrided methods

    /*! Renders the ball item.
      * @param[in] painter the painter
      * @param[in] options the parameters used to paint this graphic item
      * @param[in] widget the widget that is painted on
      */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget);

    //! end of the overrided methods

    /*! Sets the painting context (color and brush) used to paint the ball item.
      *
      * @param[in] paintCntx the painting context
      */
    inline void setPaintCntx(const QSharedDataPointer<BallItemPaintCntx> &paintCntx)
    {
        m_paintCntx = paintCntx;
    }

    /*!
      * @return the painting context
      */
    inline BallItemPaintCntx paintCntx() const
    {
        return *m_paintCntx;
    }

    /*!
      * @return the color used to paint the ball item
      */
    inline QColor color() const
    {
        return m_paintCntx->color();
    }

    /*!
      * @return the grid coordinates of a ball item
      * \sa row(), column(), setCoordinates()
      */
    inline GridPos coordinates() const
    {
        return m_coord;
    }

    /*!
      * @return the row in the grid where this ball item is rendered on
      * \sa coordinates(), column(), setCoordinates()
      */
    inline int row() const
    {
        return m_coord.row();
    }

    /*!
      * @return the column in the grid where this ball item is rendered on
      * \sa coordinates(), row(), setCoordinates(int, int), setCoordinates(const GridPos &)
      */
    inline int column() const
    {
        return m_coord.column();
    }

    /*! Sets the grid coordinates of a ball item.
      * @param[in] coord the coordinates of a ball item
      * \sa coordinates(), setCoordinates(int, int)
      */
    inline void setCoordinates(const GridPos &coord)
    {
        m_coord = coord;
    }

    /*! Sets the grid coordinates of a ball item.
      *
      * @param[in] row the row
      * @param[in] col the column
      * \sa coordinates(), setCoordinates(const GridPos &)
      */
    inline void setCoordinates(int row, int col)
    {
        m_coord.setRow(row);
        m_coord.setColumn(col);
    }

    /*!
      * @return true if this is a hint ball, false otherwise
      * \sa setHint()
      */
    inline bool isHint() const
    {
        return m_hintFlag;
    }

    /*!
      * @param[in] flag the hint falg
      * \sa isHint()
      */
    inline void setHint(bool flag = true)
    {
        m_hintFlag = flag;
    }

    /*!
      * @return true if the ball item is selected, false otherwise
      * \sa select()
      */
    inline bool isSelected() const
    {
        return m_selectedFlag;
    }

    /*! Sets a ball item as being selected or unselected.
      * @param[in] flag
      * @param[in] refresh
      * \sa isSelected()
      */
    inline void select(bool flag, bool refresh = false)
    {
        m_selectedFlag = flag;
        if (refresh) {
            update();
        }
    }

private:
    qreal m_radius; /*!< the radius of the ball */
    QSharedDataPointer<BallItemPaintCntx> m_paintCntx; /*!< the painting context (color and brush) */

    GridPos m_coord; /*!< the position of the ball in the grid's coordinates */
    bool   m_hintFlag; /*!< is the ball a hint one ? */
    bool   m_selectedFlag; /*!< is the ball selected ? */
};

#endif // BALLITEM_HPP
