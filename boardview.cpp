/*!
  * @file BoardView.cpp
  * This file contains the definition of the class BoardView.
  */

#include <QGraphicsScene>
#include "boardview.hpp"
#include "ballitem.hpp"
#include "ballitemsprovider.hpp"
#include "pathfinder.hpp"
#include "mainwidget.hpp"
#include "utils.hpp"

/*!
  */
BoardView::BoardView(QWidget *parent)
    : QGraphicsView(parent)
{
    setCacheMode(CacheBackground);

    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    setBackgroundBrush(QColor(230, 200, 167));

    if (isRunningOnDesktop()) {
        // run on PC desktop.
        m_scene = new QGraphicsScene(-225, -225, 450, 450);
    } else {
        // run on a phone/mobile device.
        m_scene = new QGraphicsScene(-90, -90, 180, 180);
    }
    setScene(m_scene);

    m_grid = new GridItem(9);
    m_scene->addItem(m_grid);

    //
    PathFinder::instance()->init(m_grid->dim());

    // initializes the ball items provider
    BallItemsProvider::instance()->init(m_grid);
    //

    BallItemsProvider::instance()->nextBalls(true);
}

/*!
  */
BoardView::~BoardView()
{
    delete m_scene;
}

/*!
  */
void BoardView::reset()
{
    Q_ASSERT(m_grid != 0);

    m_grid->reset();
    MainWidget::instance()->resetScore();

    BallItemsProvider::instance()->reset();
    BallItemsProvider::instance()->nextBalls(true);
}
