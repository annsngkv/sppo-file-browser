#include "grouper.h"

/* Группируем, вычисляем размер и отображаем результат*/
void Grouper::grouping(QString const& path, QDir::Filters filters)
{
    QMap<QString, QPair<qint64, qreal>> files_info;
    files_info = grouper->calculateAndGroup(path, filters);

    grouper->displayFilesInfo(files_info);
}