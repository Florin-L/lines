/*!
  * @file buttonsview.cpp
  * This file contains the definition of the class ButtonsView.
  */

#include <QHBoxLayout>
#include <QtGui/QFont>
#include "buttonsview.hpp"

//!
ButtonsView::ButtonsView(QWidget *parent)
        : QWidget(parent)
{
    QFont font("Times", 24, QFont::Bold);
    QHBoxLayout *layout = new QHBoxLayout;

    QLabel *label = new QLabel(tr("Score: "));
    label->setFont(font);
    label->setMaximumSize(150, 50);
    layout->addWidget(label);

    m_score = new QLabel(this);
    m_score->setAlignment(Qt::AlignLeft);
    m_score->setFont(font);
    m_score->setMaximumSize(150, 50);
    m_score->setText("0");
    layout->addWidget(m_score);

    setLayout(layout);
    show();
}
