#include "filebrowserdatamodel.h"

//В конструкторе инициализируем исходные данные.
FileBrowserDataModel::FileBrowserDataModel(QObject *parent, QList<SomeData> dt) : QAbstractTableModel(parent) {
    dataModel = dt;
}

//Возвращаем количество строк, в зависимости от количества данных в списке
int FileBrowserDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return dataModel.count();
}

//Возвращаем количество столбцов, оно у нас постоянно
int FileBrowserDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return PERCENT + 1;
}

//Возвращаем названия заголовков. Обратите внимание на тип возвращаемого значения.
QVariant FileBrowserDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Vertical) {
        return section;
    }

    switch (section) {
        case NAME:
            return trUtf8("Название");
        case SIZE:
            return trUtf8("Размер");
        case PERCENT:
            return trUtf8("В процентах");
    }

    return QVariant();
}

//Возвращаем соответствующие данные, относительно модельного индекса и роли.
//В случае, если на вход пришли не корректные данные, что может возникнуть при не
//верной реализации, также в случае, когда приходит роль (int role), которую нам не
//нужно обрабатывать, в частности Qt::DisplayRole и Qt::EditRole , возвращается пустой
//объект QVariant(). Тем самым обеспечивается устойчивая работа представления, при
//отображении модели в случае наличия ошибок.
// Модельный индекс характеризуется номером строки и столбца, следовательно в зависимости
//от номера и столбца возвращаются корректные данные, сохраненные в модели данных.
QVariant FileBrowserDataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || dataModel.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
        return QVariant();
    }

    if (index.column() == 0) {
        return dataModel[index.row()].name;
    } else if (index.column() == 1) {
        return dataModel[index.row()].size;
    } else if (index.column() == 2) {
        return dataModel[index.row()].percent;
    }
}
