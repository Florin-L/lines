/*!
  * @file buttonsview.hpp
  *
  * This file contains the declaration of the class ButtonsView.
  */

#ifndef BUTTONSVIEW_HPP
#define BUTTONSVIEW_HPP

#include <QWidget>
#include <QLabel>
#include <QtCore/QString>

/*! This class implements a panel that holds the control widgets.
  */
class ButtonsView : public QWidget
{
    Q_OBJECT
public:
    /*! The constructor.
      * @param[in] parent the parent of this widget
      */
    ButtonsView(QWidget *parent = 0);

    //!
    inline void updateScore(const QString &value);

protected:
    QLabel      *m_score; /*!< the widget that displays the score */
};

//
inline void ButtonsView::updateScore(const QString &value)
{
    m_score->setText(value);
}

#endif // BUTTONSVIEW_HPP
