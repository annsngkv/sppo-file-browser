#ifndef GROUPER_H
#define GROUPER_H
#include <QMap>
#include "iostream"
#include "memory"
#include "ifilegrouping.h"

/* Класс контекст */
class Grouper
{
    /* Указатель на стратегию*/
    std::shared_ptr<IFileGrouping> grouper;

    public:
        /* Конструктор с параметрами */
        Grouper(const std::shared_ptr<IFileGrouping> &_grouper) {grouper = _grouper;}
        /*
         * Указываем компилятору самостоятельно сгенерировать деструктор по-умолчанию
         */
        ~Grouper() = default;
        /* Функция для изменения стратегии */
        void changeStrategy(const std::shared_ptr<IFileGrouping> &_grouper) {grouper = _grouper;}
        /* Функция группирования файлов и вычиления размера групп в зависимости от стратегии*/
        QList<SomeData> grouping(QString const& path, QDir::Filters filters);
};

#endif // GROUPER_H
