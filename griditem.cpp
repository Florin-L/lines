/*!
* @file griditem.cpp
* This file contains the implementation of the class GridItem.
*/

#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QEventLoop>
#include <QtCore/QCoreApplication>
#include "ballitem.hpp"
#include "griditem.hpp"
#include "gridpos.hpp"
#include "pathfinder.hpp"
#include "boardview.hpp"
#include "linestracker.hpp"
#include "mainwidget.hpp"
#include "ballitemsprovider.hpp"
#include "utils.hpp"

//!
GridItem::GridItem(int dimension)
    : m_dimension(9),
    m_penWidth(1),
    m_ballSelected(false)
{
    m_squareSize = isRunningOnDesktop() ? 50 : 20;

    for (int i = 0; i < dimension; ++i) {
        memset(reinterpret_cast<void*>(&m_balls[i]), 0, sizeof(int) * m_dimension);
    }

    m_size = m_dimension * m_dimension;
}

//!
GridItem::~GridItem()
{
    reset();
}

/*!
* Computes the bounding rect of the grid.
*/
QRectF GridItem::boundingRect() const
{
    QGraphicsScene *grscene = scene();
    if (grscene) {
        qreal w = grscene->width();
        qreal h = grscene->height();

        return QRectF((-w - m_penWidth)/2, (-h - m_penWidth)/2, w + m_penWidth, h + m_penWidth);
    }

    return QRectF();
}

/*!
* Renders the grid onto the screen.
*/
void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
    Q_UNUSED(options);
    Q_UNUSED(widget);

    painter->setPen(QPen(QColor(255, 0, 255), m_penWidth));
    painter->drawRect(boundingRect());

    QRectF rc = boundingRect();

    // renders the horizontal lines
    int y1 = int(rc.y());
    int y2 = y1 + m_squareSize * m_dimension;
    for (int i = 1; i < m_dimension; ++i) {
        int x = int(i * m_squareSize + rc.x());
        painter->drawLine(x, y1, x, y2);
    }

    // renders the vertical lines
    int x1 = int(rc.x());
    int x2 = x1 + m_squareSize * m_dimension;
    for (int i = 1; i < m_dimension; ++i) {
        int y = int(i * m_squareSize + rc.y());
        painter->drawLine(x1, y, x2, y);
    }

    // render the path tracker
    if (m_pathTracker.points().count()) {
        painter->setPen(QPen(Qt::black, m_penWidth));
        painter->drawLines(m_pathTracker.points());
    }
}

/*!
* Resets the content of the grid.
* Hides all the ball items from the board and removes them
* from the internal structure.
*/
void GridItem::reset()
{
    resetAnimation();

    for (int row = 0; row < m_dimension; ++row) {
        for (int col = 0; col < m_dimension; ++col) {
            BallItem * pBall = m_balls[row][col];
            if (pBall) {
                pBall->setVisible(false);
                pBall->setParentItem(0);

                delete m_balls[row][col];
                m_balls[row][col] = reinterpret_cast<BallItem*>(0);
            }
        }
    }
}

/*!
* Displays the ball item in a given location onto the grid.
*/
void GridItem::showBall(BallItem *ball, int row, int col, bool updateInternalStruct)
{
    if (!ball) {
        qDebug() << "GridItem::showBall(...): pointer to the ball item is NULL";
        return;
    }

    Q_ASSERT(isValidPosition(row, col));

    ball->setCoordinates(row, col);
    if (!ball->isVisible()) {
        ball->setVisible(true);
    }

    if (updateInternalStruct) {
        m_balls[row][col] = ball;

        // available to used
        BallItemsProvider::instance()->fromAvailableToUsed(ball->coordinates());
        //
    }

    QPoint pt;
    fromGridToCenteredCoordinate(row, col, pt);

    ball->setPos(pt.x(), pt.y());
}

/*!
*/
void GridItem::showBall(BallItem *ball, const GridPos &pos, bool updateInternalStruct)
{
    showBall(ball, pos.row(), pos.column(), updateInternalStruct);
}

/*!
*/
void GridItem::showBall(BallItem *ball)
{
    showBall(ball, ball->coordinates());
}

/*!
*/
BallItem* GridItem::hideBall(BallItem *ball)
{
    if (!ball) {
        qDebug() << "GridItem::hideBall(BallItem *): pointer to the ball item is NULL";
        return 0;
    }

    ball->setVisible(false);
    // changes used position to an available one
    BallItemsProvider::instance()->fromUsedToAvailable(ball->coordinates());
    //

    freePos(ball->coordinates());
    return ball;
}

/*!
*/
void GridItem::moveBall(BallItem *ball, const QVector<GridPos> &path)
{
    if (!ball) {
        qDebug() << "GridItem::moveBall(...): pointer to the ball item is NULL";
        return;
    }

    QVector<GridPos> tmpPath(path);

    // skip the first square
    GridPos firstPos = tmpPath.front();
    freePos(firstPos);
    //

    // the source square becomes available
    BallItemsProvider::instance()->fromUsedToAvailable(firstPos);
    //

    QGraphicsScene *theScene = scene();
    Q_ASSERT(theScene);
    Q_ASSERT(theScene->views().isEmpty() == false);

    theScene->views()[0]->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    foreach(GridPos pos, tmpPath) {
        if (pos != tmpPath.back()) {
            // do not store the ball item on the positions of the intermediate squares (the 3rd argument
            // is set to false).
            showBall(ball, pos, false);
            pause(100);
        } else {
            // is there any hint ball onto the target square ?
            BallItem *hintBall = ballAt(pos);
            if (hintBall) {
                hideBall(hintBall);
                delete hintBall;

                BallItemsProvider::instance()->removeHint(hintBall);
            }

            // stores the ball item on the position of the target square.
            showBall(ball, pos, true);
        }
        // also wipes the path tracks square by square
        m_pathTracker.removeFrontLine();
        update();
    }
}

/*!
*/
void GridItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    GridPos pt;
    fromViewToGridCoordinate(event->pos(), pt);

    if (!m_ballSelected && !isFreePos(pt)) {
        m_ballSelected = true;
        m_beginPos = pt;
        selectBall(m_beginPos);
    }
}

/*!
*/
void GridItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_ballSelected) {
        event->ignore();
        return;
    }

    GridPos pt;
    fromViewToGridCoordinate(event->pos(), pt);

    if (m_beginPos == pt) {
        m_ballSelected = false;
        selectBall(pt, false);

        // clear the path tracker and repaint the grid
        m_pathTracker.clear();
        update();
        //
    } else if (isValidPosition(pt) && isFreePos(pt)) {
        QVector<GridPos>& path = m_pathTracker.path();
        if (!path.isEmpty()) {
            // have we moved onto a square occupied by a hint ball ? if we have then a new set of the hint balls
            // needs to be generated.
            BallItem *ball = ballAt(path.back());
            bool enforceHintBalls = (ball && ball->isHint());
            //

            moveBall(ballAt(m_beginPos), path);

            // unselect the moving ball
            ball = ballAt(path.back());
            ball->select(false);
            m_ballSelected = false;

            // searches for the lines of the same ball items that have the same colours.
            checkLines(ball);

            // do we have more available positions ? if we do then proceed with a new set of ball items.
            if (!BallItemsProvider::instance()->availableIndexes().isEmpty()) {
                // get the next set of balls
                const QList<BallItem*> &currentBalls = BallItemsProvider::instance()->nextBalls(enforceHintBalls);
                // searches for the lines of the same ball items that have the same colours.
                checkLines(currentBalls);
            } 
            
            // even more available positions ? if not then quit or reset the game.
            if (BallItemsProvider::instance()->availableIndexes().isEmpty()){
                if (promptForGameEnd()) {
                    QCoreApplication::quit();
                } else {
                    MainWidget::instance()->boardView()->reset();
                } 
            }
        }
    }
}

/*!
*/
void GridItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_ballSelected) {
        event->ignore();
        return;
    }

    GridPos pt;
    fromViewToGridCoordinate(event->pos(), pt);

    if (isValidPosition(pt) && isFreePos(pt) && (pt != m_beginPos)) {
        m_pathTracker.clear();

        QVector<GridPos>& path = m_pathTracker.path();
        bool found = PathFinder::instance()->execute(this, m_beginPos, pt, path);

        if (found && (path.count() >= 2)) {
            int n = path.count();
            for (int i = 0; i < n-1; ++i) {
                QPoint pt1;
                fromGridToCenteredCoordinate(path.at(i), pt1);

                QPoint pt2;
                fromGridToCenteredCoordinate(path.at(i+1), pt2);

                m_pathTracker.addLine(pt1, pt2);
            }

            update(); // repaint the grid
        }
    }
}

/*!
*/
void GridItem::checkLines(BallItem *ball)
{
    QList<BallItem*> *list = new QList<BallItem*>;
    list->push_back(ball);

    checkLines(*list);

    delete list;
}

/*!
*/
void GridItem::checkLines(const QList<BallItem*> &balls)
{
    if (0 == balls.count())
        return;

    LinesTracker::set_of_positions_t *positions = new LinesTracker::set_of_positions_t;

    int n = balls.count();
    for (int i = 0; i < n; ++i) {
        LinesTracker::instance()->search(this, balls[i]);

        if (LinesTracker::instance()->horizontalLine().count() >= 5)
            insertIntoPositionSet(*positions, LinesTracker::instance()->horizontalLine());

        if (LinesTracker::instance()->verticalLine().count() >= 5)
            insertIntoPositionSet(*positions, LinesTracker::instance()->verticalLine());

        if (LinesTracker::instance()->diag1Line().count() >= 5)
            insertIntoPositionSet(*positions, LinesTracker::instance()->diag1Line());

        if (LinesTracker::instance()->diag2Line().count() >= 5)
            insertIntoPositionSet(*positions, LinesTracker::instance()->diag2Line());

        LinesTracker::instance()->clear();
    }

    if (positions->count()) {
        animateBalls(*positions);

        int score = removeLines(*positions);
        MainWidget::instance()->updateScore(score);
    }

    delete positions;
}

/*!
*/
int GridItem::removeLines(LinesTracker::set_of_positions_t &positions)
{
    int score = 0;

    foreach (GridPos pos, positions) {
        BallItem *ball = hideBall(pos);
        if (ball)
            delete ball;
    }

    int n = positions.size();
    score += (n-1) * 150;

    return score;
}

/*!
*/
void GridItem::animateBalls(LinesTracker::set_of_positions_t &positions)
{
    bool selected = true;

    int steps = 5;
    while (--steps >= 0) {
        foreach (GridPos pos, positions) {
            ballAt(pos)->select(selected);
        }
        update();

        selected = !selected;
        pause(150);
    }
}

/*!
*/
void GridItem::resetAnimation()
{
    bool selected = true;
    int steps = 5;

    while (--steps >= 0) {
        for (int i = 0; i < dim(); ++i) {
            for (int j = 0; j < dim(); ++j) {
                if (ballAt(i, j)) {
                    ballAt(i, j)->select(selected);
                }
            }
        }
        update();

        selected = !selected;
        pause(150);
    }
}

/*!
*/
void GridItem::fromGridToCenteredCoordinate(int row, int col, QPoint &gridPos)
{
    if ((row < 0) || (row >= m_dimension) ||
        (col < 0) || (col >= m_dimension)) {
        qDebug() << "The coordinates are outside of the grid dimensions.";
        return;
    }

    QRectF rc = boundingRect();

    int x0 = int(rc.x());
    int y0 = int(rc.y());

    gridPos.setX(x0 + m_squareSize * col + m_squareSize/2);
    gridPos.setY(y0 + m_squareSize * row + m_squareSize/2);
}

/*!
*/
void GridItem::fromGridToCenteredCoordinate(const GridPos &pos, QPoint &centerPos)
{
    fromGridToCenteredCoordinate(pos.row(), pos.column(), centerPos);
}

/*!
* x : the column's coordinate
* y : the row's coordinate
*/
void GridItem::fromViewToGridCoordinate(int x, int y, GridPos &gridCoord)
{
    QRectF rc = boundingRect();

    int col = (int(rc.x()) + x) / m_squareSize + m_dimension - 1;
    int row = (int(rc.y()) + y) / m_squareSize + m_dimension - 1;

    gridCoord.setColumn(col); // column's coordinate is mapped to property 'y'
    gridCoord.setRow(row); // row's coordinate is mapped to property 'x'
}

/*!
*
*/
void GridItem::fromViewToGridCoordinate(const QPoint &viewCoord, GridPos &gridCoord)
{
    fromViewToGridCoordinate(viewCoord.x(), viewCoord.y(), gridCoord);
}

/*!
*/
void GridItem::fromViewToGridCoordinate(const QPointF &viewCoord, GridPos &gridCoord)
{
    fromViewToGridCoordinate(int(viewCoord.x()),
                             int(viewCoord.y()),
                             gridCoord);
}

/*!
*/
void GridItem::pause(int ms, bool ignoreUserEvents)
{
    QFlags<QEventLoop::ProcessEventsFlag> processEventsFlag = QEventLoop::AllEvents;

    if (ignoreUserEvents) {
        processEventsFlag |= QEventLoop::ExcludeUserInputEvents;
    }

    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(processEventsFlag, 100);
}

/*!
*/
void GridItem::insertIntoPositionSet(LinesTracker::set_of_positions_t &positions, const LinesTracker::list_of_positions_t &line)
{
    foreach (GridPos pos, line) {
        positions.insert(pos);
    }
}

/*!
*/
int GridItem::promptForGameEnd()
{
    QMessageBox mb(MainWidget::instance()->boardView());
    mb.setText("No more available positions on the board.");
    mb.setInformativeText("Do you want to start a new game ?");
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb.setDefaultButton(QMessageBox::Yes);

    return (mb.exec() == QMessageBox::No);
}

void GridItem::dumpBallsMatrix()
{
    QDebug dbg(QtDebugMsg);

    dbg.nospace() << "*****\n";

    for (int i = 0; i < dim(); ++i) {
        for (int j = 0; j < dim(); ++j) {
            dbg.nospace() << (m_balls[i][j] ? (m_balls[i][j]->isHint() ? 2 : 1) : 0) << " ";
        }
        dbg.nospace() << "\n";
    }
    dbg.nospace() << "-----\n";
}
