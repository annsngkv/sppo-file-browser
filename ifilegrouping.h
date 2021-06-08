#ifndef IFILEGROUPING_H
#define IFILEGROUPING_H

#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QMap>
#include <QPair>
#include <QMapIterator>
#include <QList>
#include <somedata.h>

/* Описываем класс - интерфейс стартегии*/
class IFileGrouping {
    public:
        /*
         * Указываем компилятору самостоятельно сгенерировать деструктор по-умолчанию
         */
        virtual ~IFileGrouping() = default;
        /*
         * Функция для группирования файлов по определенной стратегии и вычисляющая размер этих групп
         * здесь определяется алгоритм, который подвержен частым изменениям
         * результат сохраняемв QMap, тип значения которого QPair<qint64, qreal>,
         * где qint64 выбран потому что функция size() QFileInfo возвращает результат этого типа,
         * a qreal нужен для хранения процентов
         *
         * arg QString const&
         * arg QDir::Filters
         *
         * return QMap<QString, QPair<qint64, qreal>>
        */
        virtual QList<SomeData> calculateAndGroup(QString const& path, QDir::Filters filters) = 0;
        /*
         * Функция для установки структуре, хранящей информацию о файлах, размера файлов в процентах
         *
         * arg QMap<QString, QPair<qint64, qreal>> &info
         *
         * return void
        */
        virtual void setPercents(QMap<QString, QPair<qint64, qreal>> &info);
        /*
         * Функция для получения строки с размером файла в килобайтах
         *
         * arg qint64
         *
         * return QString
        */
        virtual QString getKb(qint64 num);
        /*
         *  Функция вычиляющая размер файла в процентах
         *
         * arg qreal
         * arg quint64
         *
         * return qreal
        */
        virtual qreal getPercent(qreal num, qint64 totalNum);
        /*
         * Функция для получения строки с размером файла в процентах с округлением до 2-х знаков после запятой
         *
         * arg qreal
         *
         * return QString
        */
        virtual QString getPercentWithSuffix(qreal percent);
        /*
         *  Функция для получения общего размера всех файлов
         *
         * arg QMap<QString, QPair<qint64, qreal>>
         *
         * return qint64
        */
        virtual qint64 getTotalSize(QMap<QString, QPair<qint64, qreal>> const & map);
        /*
         * Функция для отображения результата группирования файлов и размер этой группы
         *
         * arg QMap<QString, QPair<qint64, qreal>>
         *
         * return void
        */
        virtual void displayFilesInfo(QList<SomeData> const &files_info);
};

#endif // IFILEGROUPING_H
