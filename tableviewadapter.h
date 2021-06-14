#ifndef TABLEVIEWADAPTER_H
#define TABLEVIEWADAPTER_H
#include "filebrowserobserver.h"
#include <QDebug>
#include <QTableView>
#include "filebrowserdatamodel.h"


/*
 * Класс, являющийся конктретным наблюдателем, он же явялется адаптером
 * для связи алгоримта вычисления размера файлов с табличным отображением данных
*/
class TableViewAdapter:public FileBrowserObserver
{
    QTableView *tableView;
    FileBrowserDataModel *tableModel;

    public:
       TableViewAdapter(QTableView *_tableView, FileBrowserDataModel *_tableModel) {
           tableView = _tableView;
           tableModel = _tableModel;
       }

       ~TableViewAdapter() {
           delete tableView;
           delete tableModel;
       }

       void displayUpdate(const QList<SomeData> &data) {
           tableModel->needReset();
           tableModel->setSomeData(data);
           tableModel->insertRow(tableModel->rowCount(tableModel->index(0, 0, QModelIndex())));
           tableView->setRootIndex(tableModel->index(0, 0, QModelIndex()));
       }
};

#endif // TABLEVIEWADAPTER_H
