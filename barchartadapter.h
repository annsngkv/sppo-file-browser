#ifndef BARCHARTADAPTER_H
#define BARCHARTADAPTER_H

#include "filebrowserobserver.h"
#include <QDebug>
#include "barchart.h"

/*
 * Класс, являющийся конктретным наблюдателем, он же явялется адаптером
 * класса, реализующего столбчатую диаграмму, для связи его с алгоримтом вычисления размера файлов
*/
class BarChartAdapter : public FileBrowserObserver
{
    QChartView *chartView;

    public:
       BarChartAdapter(QChartView *_chartView) {
           chartView = _chartView;
       }
       ~BarChartAdapter() {
           delete chartView;
       }
       void displayUpdate(const QList<SomeData> &data) {
           BarChart barChart;
           chartView->setChart(barChart.createChart(data));
       }

       QChartView * getChartView() {
           return chartView;
       }
};

#endif // BARCHARTADAPTER_H
