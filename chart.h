#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChartGlobal>
#include <QList>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include "somedata.h"
#include <QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

/*
 * Класс с шаблонным методом, реализующий интерфейс создания диаграмм
*/
class Chart
{
    public:
        /*
         * Шаблонный метод, в котором определен общий алгоритм, состоящий
         * из двух шагов.
         * 1. DrawChart(…) - Создание диаграммы
         * 2. DrawLegend(…)- Создание легенды
        */
        QChart * createChart(const QList<SomeData> &data);
        void drawLegend(QChart &chart) {
            chart.legend()->setAlignment(Qt::AlignRight);
            chart.legend()->show();
        }
    protected:
        virtual void drawChart(QChart &chart, const QList<SomeData> &data) = 0;
};
#endif // CHART_H
