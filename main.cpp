#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{    
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.resize(1200, 640);
    mainWindow.setWindowState(Qt::WindowMaximized);
    mainWindow.setWindowIcon(QIcon(":/images/logo2.png"));
    mainWindow.show();

    return app.exec();
}
