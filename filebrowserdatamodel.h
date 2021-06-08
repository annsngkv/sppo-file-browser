#ifndef FILEBROWSERDATAMODEL_H
#define FILEBROWSERDATAMODEL_H
#include <QAbstractTableModel>
#include <QList>
#include <QString>
#include "somedata.h"
class FileBrowserDataModel : public QAbstractTableModel
{
    Q_OBJECT
    public:
        FileBrowserDataModel(QObject *parent = nullptr, QList<SomeData> dt = QList<SomeData>());
        //Минимальный и обязательный набор необходимых методов
        //Так как нам требуется только отображать данные, то этого набора достаточно

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        //Функция для установки данных в модель
        void setSomeData(const QList<SomeData> &dt) {dataModel = dt;}
        //Функция для корректного обновления данных в таблице
        void needReset() {
            beginResetModel();
            endResetModel();
        }
    private:
        //Определим перечисление NameColumn для индексации столбцов нашей таблицы.
        enum NameColumn {
            NAME = 0,
            SIZE,
            PERCENT
        };
        //Представим данные модели в виде списка.
        QList<SomeData> dataModel;
};

#endif
