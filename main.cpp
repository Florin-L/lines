/*!
  * @file main.cpp
  */
#include <ctime>    // for time()
#include <QApplication>
#include <QSysInfo>
#include "mainwidget.hpp"
#include "linestracker.hpp"
#include "pathfinder.hpp"
#include "ballitemsprovider.hpp"
#include "utils.hpp"

//
bool g_isRunOnDesktop;

//
bool isRunningOnDesktop()
{
    return g_isRunOnDesktop;
}

//
void atExit(void)
{
}

int main(int argc, char *argv[])
{
    atexit(atExit);

    g_isRunOnDesktop = true;

    // sets the seed for the random number generator
    time_t seconds;
    time(&seconds);
    qsrand((unsigned int)seconds);

    QApplication a(argc, argv);

    MainWidget::instance()->show();

    return a.exec();
}
