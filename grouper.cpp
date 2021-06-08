#include "grouper.h"
#include "QDebug"

/* Группируем, вычисляем размер и отображаем результат*/
QList<SomeData> Grouper::grouping(QString const& path, QDir::Filters filters)
{
    QList<SomeData> files_info;
    try {
        files_info = grouper->calculateAndGroup(path, filters);

        return files_info;
    } catch(std::runtime_error err) {
        qDebug() << err.what();
    }
}
