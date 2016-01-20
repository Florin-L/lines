/*!
  * @file ballitemsprovider.hpp
  * This file contains the definition of the class BallItemsProvider.
  */

#include "ballitemsprovider.hpp"


/*!
  */
void BallItemsProvider::reset()
{
    if (m_hintBalls.count() > 0)
        m_hintBalls.clear();    
    
    if (m_currentBalls.count() > 0)
        m_currentBalls.clear();

    if (m_usedIdxs.count())
        m_usedIdxs.clear();

    if (m_availableIdxs.count())
        m_availableIdxs.clear();

    for (int i = 0; i < m_grid->dim(); ++i)
        for (int j = 0; j < m_grid->dim(); ++j)
            m_availableIdxs.push_back(i*9 + j);
}

/*!
  */
void BallItemsProvider::init(GridItem *grid)
{
    m_grid = grid;

    m_colors.reserve(5);

    // red
    QRadialGradient gradientRed(QPointF(0, 0), 0);
    QColor clrRed(Qt::red);
    gradientRed.setColorAt(0.02, clrRed.lighter());
    gradientRed.setColorAt(0.98, clrRed);

    m_colors.push_back(QSharedDataPointer<BallItemPaintCntx>(new BallItemPaintCntx(clrRed, gradientRed)));

    // blue
    QRadialGradient gradientBlue(QPointF(0, 0), 0);
    QColor clrBlue(Qt::blue);
    gradientBlue.setColorAt(0.02, clrBlue.lighter());
    gradientBlue.setColorAt(0.98, clrBlue);

    m_colors.push_back(QSharedDataPointer<BallItemPaintCntx>(new BallItemPaintCntx(clrBlue, gradientBlue)));

    // green
    QRadialGradient gradientGreen(QPointF(0, 0), 0);
    QColor clrGreen(Qt::green);
    gradientGreen.setColorAt(0.02, clrGreen.lighter());
    gradientGreen.setColorAt(0.98, clrGreen);

    m_colors.push_back(QSharedDataPointer<BallItemPaintCntx>(new BallItemPaintCntx(clrGreen, gradientGreen)));

    // yellow
    QRadialGradient gradientYellow(QPointF(0, 0), 0);
    QColor clrYellow(Qt::yellow);
    gradientYellow.setColorAt(0.02, clrYellow.lighter());
    gradientYellow.setColorAt(0.98, clrYellow);

    m_colors.push_back(QSharedDataPointer<BallItemPaintCntx>(new BallItemPaintCntx(clrYellow, gradientYellow)));

    // magenta
    QRadialGradient gradientMagenta(QPointF(0, 0), 0);
    QColor clrMagenta(Qt::magenta);
    gradientMagenta.setColorAt(0.02, clrMagenta.lighter());
    gradientMagenta.setColorAt(0.98, clrMagenta);

    m_colors.push_back(QSharedDataPointer<BallItemPaintCntx>(new BallItemPaintCntx(clrMagenta, gradientMagenta)));

    reset();
}

/*!
  */
BallItem* BallItemsProvider::createBall()
{
    BallItem *ball = new BallItem();
    ball->setPaintCntx(m_colors[qrand()%5]);

    return ball;
}

/*!
  */
void BallItemsProvider::removeHint(BallItem *ball)
{
    int pos = m_hintBalls.indexOf(ball);
    if (pos != -1) {
        m_hintBalls[pos] = reinterpret_cast<BallItem*>(0);
    }
}

/*!
  */
const QList<BallItem*>& BallItemsProvider::nextBalls(bool enforceHints)
{
    m_currentBalls.clear();

    if (m_availableIdxs.isEmpty()) {
        return m_currentBalls;
    }

    if (enforceHints && !m_hintBalls.isEmpty()) {
        // prepares the list that stores the hint balls for a new set of balls
        for (int i = 0; i < m_hintBalls.count(); ++i) {
            BallItem *pBall = m_hintBalls[i];
            if (pBall && pBall->isHint()) {
                m_grid->hideBall(pBall);
            }
            delete pBall;
        }
        m_hintBalls.clear();
    }

    //qDebug() << "available before: " << m_availableIdxs.size();

    if (m_hintBalls.isEmpty()) {
        //
        // If the list of the hint ball is empty then a new set of balls has to be generated.
        //
        int nidxs = (m_availableIdxs.count() >= 3) ? 3 : (3 - m_availableIdxs.count()); 
        for (int i = 0; i < nidxs; ++i) {
            int n = m_availableIdxs.count();
            int pos = qrand()%n;

            // creates a new ball item and sets its coordinates and the parent item
            BallItem *ball = createBall();

            int index = m_availableIdxs.at(pos);

            ball->setCoordinates(index / m_grid->dim(), index % m_grid->dim());
            ball->setParentItem(m_grid);

            m_currentBalls.push_back(ball);
            m_grid->showBall(ball);

            if (m_availableIdxs.isEmpty()) {
                return m_currentBalls;
            }
        }
    } else {
        // The set of the hint balls has been already built. Just change them into the normal ones.
        for (int i = 0; i < m_hintBalls.count(); ++i) {
            m_hintBalls[i]->setHint(false);
            m_currentBalls.push_back(m_hintBalls[i]);

            fromAvailableToUsed(m_hintBalls[i]->coordinates());
        }
        m_hintBalls.clear();
        m_grid->update();
    }

    // Build the set of the 'hint' balls.
    Q_ASSERT(m_hintBalls.isEmpty());

    for (int i = 0; i < 3; ++i) {
        if (m_availableIdxs.isEmpty()) {
            break;
        }

        int n = m_availableIdxs.count();
        int pos = qrand()%n;

        // creates a new hint ball item and sets its coordinates, the parent item and the hint flag
        BallItem *ball = createBall();

        int index = m_availableIdxs.at(pos);

        ball->setCoordinates(index / m_grid->dim(), index % m_grid->dim());
        ball->setHint(true);
        ball->setParentItem(m_grid);

        m_grid->showBall(ball);

        m_hintBalls.push_back(ball);
    }

    // the indexes of the hint balls were marked temporarly as being used just for the generating
    // of the hint's positions randomly
    for (int i = 0; i < m_hintBalls.count(); ++i)
        fromUsedToAvailable(m_hintBalls[i]->coordinates());

    //qDebug() << "available indexes after: " << m_availableIdxs.size();
    Q_ASSERT(m_availableIdxs.size() + m_usedIdxs.size() == m_grid->size());

    return m_currentBalls;
}

/*!
  */
void BallItemsProvider::fromAvailableToUsed(const GridPos &gridpos)
{
    int index = toUnidimensionalIndex(gridpos);

    if (m_availableIdxs.removeOne(index)) {
        m_usedIdxs.push_back(index);
    }
}

/*!
  */
void BallItemsProvider::fromUsedToAvailable(const GridPos &gridpos)
{
    int index = toUnidimensionalIndex(gridpos);
    if (m_usedIdxs.removeOne(index)) {
        m_availableIdxs.push_back(index);
    }
}
