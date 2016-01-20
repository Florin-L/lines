/*!
  * @file boardview.hpp
  *
  * This file contains the declaration of the class BoardView.
  */

#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include <QGraphicsView>
#include "griditem.hpp"

// forward declarations
class QGraphicsScene;

/*! This class implements view that owns the grid item where the ball items are to be rendered on.
  */
class BoardView : public QGraphicsView
{
    Q_OBJECT
public:
    /*! The constructor.
      * @param[in] parent the parent widget
      */
    BoardView(QWidget *parent = 0);

    /*! The destructor.
      */
    ~BoardView();

public Q_SLOTS:
    /*! Reinitializes the board : remove all the ball items and reset the internal structure.
      */
    void reset();

protected:
    GridItem *m_grid; /*!< the grid item */
    QGraphicsScene *m_scene; /*!< the graphics scene */
};

#endif // BOARDVIEW_HPP
