/*!
  * \file mainwidget.cpp
  */

#include <QDockWidget>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QtCore/QCoreApplication>
#include "mainwidget.hpp"

//
template<>
MainWidget* Singleton<MainWidget>::s_instance = NULL;

//
MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    m_btnsView(NULL),
    m_score(0)
{
    setWindowTitle(QObject::tr("Line Balls"));

    // create the board
    m_board = new BoardView(this);

    // initialize the menu bar
    QMenu *game = new QMenu(tr("Game"));

    QAction *reset = new QAction(tr("Restart"), this);
    reset->setWhatsThis(tr("Restart the game"));
    reset->setShortcut(QKeySequence(tr("CTRL+R")));
    connect(reset, SIGNAL(triggered()), m_board, SLOT(reset()));

    QAction *exit = new QAction(tr("Exit"), this);
    exit->setWhatsThis(tr("Quit the game"));
    exit->setShortcut(QKeySequence(tr("ALT+X")));
    connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));

    game->addAction(reset);
    game->addAction(exit);

    menuBar()->addMenu(game);

    // create the buttons or the menu bar depending on the target we want to run it (PC or phone).
    if (isRunningOnDesktop()) {
        // the target is the a desktop not a phone/mobile/embedded device.
        QDockWidget *dockWidget = new QDockWidget(this);
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

        m_btnsView = new ButtonsView(dockWidget);
        dockWidget->setWidget(m_btnsView);

        addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    }

    setCentralWidget(m_board);
}
