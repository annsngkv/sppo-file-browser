#ifndef PIECHARTADAPTER_H
#define PIECHARTADAPTER_H
#include "filebrowserobserver.h"
#include <QDebug>
#include "piechart.h"

/*
 * Класс, являющийся конктретным наблюдателем, он же явялется адаптером
 * класса, реализующего круговою диаграмму, для связи его с алгоримтом вычисления размера файлов
*/
class PieChartAdapter : public FileBrowserObserver
{
    QChartView *chartView;

    public:
       PieChartAdapter(QChartView *_chartView) {
           chartView = _chartView;
       }
       ~PieChartAdapter() {
           delete chartView;
       }
       void displayUpdate(const QList<SomeData> &data) {
           PieChart pieChart;
           chartView->setChart(pieChart.createChart(data));
       }

       QChartView * getChartView() {
           return chartView;
       }
};

#endif // PIECHARTADAPTER_H
