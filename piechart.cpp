#include "piechart.h"
#include <QString>
#include <QRegularExpression>
#include <QDebug>
#include <QChart>

//Метод для создания круговой диаграммы
void PieChart::drawChart(QChart &chart, const QList<SomeData> &data)
{
    QPieSeries *pieSeries = new QPieSeries();

    for (int i(0); i < data.count(); i++) {
        QString copy_data = data[i].size;
        copy_data.chop(2);
        pieSeries->append(data[i].name + " (" + data[i].percent + ")", copy_data.toDouble());
    }

    chart.addSeries(pieSeries);
}
