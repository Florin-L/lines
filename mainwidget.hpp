/*!
  * @file mainwidget.hpp
  * This file contains the declaration of the class MainWidget.
  */
#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QMainWindow>
#include <QSysInfo>
#include "boardview.hpp"
#include "buttonsview.hpp"
#include "singleton.hpp"
#include "utils.hpp"

/*! Implements the main window of the application.
  */
class MainWidget :
        public QMainWindow,
        public Singleton<MainWidget>
{
    friend class Singleton<MainWidget>;

public:

    /*!
      * @return the board view
      */
    inline BoardView *boardView();

    /*!
      * @return the buttons view (the 'control panel')
      */
    inline ButtonsView *buttonsView();

    /*! Updates the score.
      * @param[in] value the value to increase the score by
      */
    inline void updateScore(int value);

    /*! Resets the score.
      */
    inline void resetScore();

private:
    /*! The constructor.
      * @param[in] parent the parent of this widget
      */
    MainWidget(QWidget *parent = 0);

    MainWidget(const MainWidget &);
    MainWidget operator =(const MainWidget &);

private:
    BoardView *m_board; /*!< the board view */
    ButtonsView *m_btnsView; /*!< the 'control panel' */

    int m_score;
};

//
inline BoardView * MainWidget::boardView()
{
    return m_board;
}

//
inline ButtonsView * MainWidget::buttonsView()
{
    return m_btnsView;
}

//
inline void MainWidget::updateScore(int value)
{
    m_score += value;
    QString score = QString("%1").arg(m_score);

    if (isRunningOnDesktop()) {
        m_btnsView->updateScore(score);
    } else {
        setWindowTitle(tr("Line Balls - ").append(score));
    }
}

//
inline void MainWidget::resetScore()
{
    if (isRunningOnDesktop()) {
        m_btnsView->updateScore("0");
    } else {
        setWindowTitle(tr("Line Balls"));
    }
}

#endif // MAINWIDGET_HPP
