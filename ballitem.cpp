/*!
  * @file ballitem.cpp
  * This file contains the definition of the class BallItem.
  */

#include <QtGui>
#include "ballitem.hpp"
#include "mainwidget.hpp"
#include "utils.hpp"

//!
BallItem::BallItem(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
    m_hintFlag(false),
    m_selectedFlag(false)
{
    setPen(Qt::NoPen);

    m_radius = isRunningOnDesktop() ? 16.0 : 6.4;

    qreal radius = m_radius * 1.2;
    qreal diameter = 2.0 * radius;
    setRect(-radius, -radius, diameter, diameter);
}

//!
void BallItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
    Q_UNUSED(options);
    Q_UNUSED(widget);

    qreal radius = m_radius;

    if (m_hintFlag)
        radius /= 3;
    else if (m_selectedFlag)
        radius *= 1.2;

    qreal diameter = 2 * radius;
    setRect(-radius, -radius, diameter, diameter);

    m_paintCntx->gradient().setRadius(radius);
    setBrush(m_paintCntx->gradient());

    QGraphicsEllipseItem::paint(painter, options, widget);
}
