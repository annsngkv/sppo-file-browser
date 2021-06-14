#ifndef FILEBROWSEROBSERVER_H
#define FILEBROWSEROBSERVER_H
#include <QList>
#include "somedata.h"

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

/*
 * Класс, представляющий интерфейс включающий метод (displayUpdate), который должен вызываться
 * при каждом изменении.
*/
class FileBrowserObserver
{
    public:
        virtual ~FileBrowserObserver() = default;
        virtual void displayUpdate(const QList<SomeData> &data) = 0;
};

#endif // FILEBROWSEROBSERVER_H
