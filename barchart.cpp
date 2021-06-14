#include "barchart.h"
#include <QString>
#include <QRegularExpression>
#include <QDebug>
#include <QChart>

//Метод для создания столбчатой диаграммы
void BarChart::drawChart(QChart &chart, const QList<SomeData> &data)
{
    QBarSeries *barSeries = new QBarSeries(&chart);

    for (int i(0); i < data.count(); i++) {
        QString copy_data = data[i].size;
        copy_data.chop(2);
        QBarSet *set = new QBarSet(data[i].name + " (" + data[i].percent + ")");
        set->append(copy_data.toDouble());
        barSeries->append(set);
    }

    chart.addSeries(barSeries);
}
