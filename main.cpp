#include <QApplication>
#include <QtWidgets>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
//Создаем модельные данные и заполняем их
    MainWindow main;

    main.show();


return app.exec();
}
