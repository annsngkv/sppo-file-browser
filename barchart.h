#ifndef BARCHART_H
#define BARCHART_H

#include "chart.h"

/*
 * Класс для создания столбчатой диаграммы, переопределяющий метод drawChart
*/
class BarChart : public Chart
{
    void drawChart(QChart &chart, const QList<SomeData> &data);
};

#endif // BARCHART_H
