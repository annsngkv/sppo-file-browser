#include "chart.h"
#include <QChart>

QChart * Chart::createChart(const QList<SomeData> &data)
{
    QChart *chart = new QChart();

    drawLegend(*chart);
    drawChart((*chart), data);

    return chart;
}
