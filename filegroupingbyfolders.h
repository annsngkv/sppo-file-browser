#ifndef FILEGROUPINGBYFOLDERS_H
#define FILEGROUPINGBYFOLDERS_H

#include "ifilegrouping.h"

/* Описывается класс с конкретной стратегией: группирование файлов по папкам*/
class FileGroupingByFolders : public IFileGrouping
{
    public:
        /*
         * Указываем компилятору самостоятельно сгенерировать конструктор по-умолчанию
         */
        FileGroupingByFolders() = default;
        /*
         * Функция для группирования файлов по папкам и вычисляющая размер этих групп
         *
         * arg const QString &
         * arg QDir::Filters
         *
         * return QMap<QString, QPair<qint64, qreal>>
         */
        QMap<QString, QPair<qint64, qreal>> calculateAndGroup(const QString &path, QDir::Filters filters);
        /*
         * Рекурсивная функция обхода папок
         *
         * arg QMap<QString, QPair<qint64, qreal>>
         * arg const QString &
         * arg QDir::Filters
         *
         * return QMap<QString, QPair<qint64, qreal>>
        */
        qint64 recursiveCalculate(qint64 &size, const QString &path, QDir::Filters filters);
};

#endif // FILEGROUPINGBYFOLDERS_H
