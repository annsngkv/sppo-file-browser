#ifndef SOMEDATA_H
#define SOMEDATA_H

/* Простой класс для описания данных модели,
 * состоящий из 3-х полей
 */
class SomeData
{
    //Поля: имя, размер, размер в процентах
    public:
        QString name;
        QString size;
        QString percent;

    public:
        //конструктор с параметрами
        SomeData(QString nm = "SomeName", QString sz = "SomeSize", QString prc = "SomePercent") {
            name = nm;
            size = sz;
            percent = prc;
        }

        //конструктор копирования
        SomeData(const SomeData &some_data) {
            name = some_data.name;
            size = some_data.size;
            percent = some_data.percent;
        }
};
#endif // SOMEDATA_H
