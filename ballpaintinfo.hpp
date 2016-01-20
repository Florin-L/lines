/*!
  * @file ballpaintinfo.hpp
  * This file contains the declaration of the class BallItemPaintCntx.
  */

#ifndef BALLPAINTINFO_HPP
#define BALLPAINTINFO_HPP

#include <QtCore/QSharedData>
#include <QtGui/QColor>
#include <QtGui/QRadialGradient>

/*! This class holds the painting context used to render a ball item.
  *
  * As many ball items may use the same color and brush to be painted on
  * this class is implemented as a shared resource that can be owned by a QSharedDataPointer.
  */
class BallItemPaintCntx : public QSharedData
{
public:
    /*! The default constructor 
    */
    BallItemPaintCntx() : m_color(), m_gradient()
    {
    }

    /*! The constructor.
      * 
      * @param[in] color the color used to initialize a gradient
      * @param[in] gradient the gradient (brush) used to fill the content of a ball item
      */
    BallItemPaintCntx(const QColor &color, const QRadialGradient &gradient)
            : m_color(color), m_gradient(gradient)
    {
    }

    /*! The destructor.
      */
    ~BallItemPaintCntx() {}

    /*!
      * @return the color
      */
    inline const QColor& color() const
    {
        return m_color;
    }

    /*!
      * @return the gradient(brush)
      */
    inline QRadialGradient& gradient()
    {
        return m_gradient;
    }

private:
    QColor m_color; /*!< the color */
    QRadialGradient m_gradient; /*!< the gradient (brush) */
};

#endif // BALLPAINTINFO_HPP
