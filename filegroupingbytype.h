#ifndef FILEGROUPINGBYTYPE_H
#define FILEGROUPINGBYTYPE_H

#include "ifilegrouping.h"

/* Описывается класс с конкретной стратегией: группирование файлов по типам(расширениям)*/
class FileGroupingByType : public IFileGrouping
{
    public:
        /*
         * Указываем компилятору самостоятельно сгенерировать конструктор по-умолчанию
         */
        FileGroupingByType() = default;
        /*
         * Функция для группирования файлов по расширениям и вычисляющая размер этих групп
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
        QMap<QString, QPair<qint64, qreal>> recursiveCalculate(QMap<QString, QPair<qint64, qreal>> &info, const QString &path, QDir::Filters filters);
};

#endif // FILEGROUPINGBYTYPE_H
