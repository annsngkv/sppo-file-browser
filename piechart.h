#ifndef PIECHART_H
#define PIECHART_H
#include "chart.h"

/*
 * Класс для создания круговой диаграммы, переопределяющий метод drawChart
*/
class PieChart : public Chart
{
    void drawChart(QChart &chart, const QList<SomeData> &data);
};

#endif // PIECHART_H
