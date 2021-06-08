#include "filegroupingbytype.h"

#include <QDebug>
#include <QtMath>

/*
 * В данной функции происходит следующее:
 * 1) Вычилсяем размер файлов, сгруппировнных по папкам
 * 2) В полученную структуру заносим информацию о размере групп в процентах
*/
QList<SomeData> FileGroupingByType::calculateAndGroup(const QString &path, QDir::Filters filters) {
    QMap<QString, QPair<qint64, qreal>> foldersInfo;

    QFileInfo file(path);

    if (file.isDir()) {
        QDir dir = file.dir();
        foldersInfo = recursiveCalculate(foldersInfo, path, filters);
        setPercents(foldersInfo);

        if (foldersInfo.empty()) {
            foldersInfo[file.fileName()] = qMakePair(0, 0.0);
        }
    } else {
        throw std::runtime_error("Not directory");
    }

    QMapIterator<QString, QPair<qint64, qreal>> f_it(foldersInfo);
    QList<SomeData> list_data;
    while (f_it.hasNext()) {
        f_it.next();
        SomeData data(f_it.key(), getKb(f_it.value().first), getPercentWithSuffix(f_it.value().second));
        list_data.append(data);
    }

    return list_data;
}

/*
 * Рекурсивная функция для группирования фалов по папкам и вычиления размера этой группы
 * В данной функции происходит следующее:
 * 1) Получем файл по указанному пути
 * 2) Проверяем, является ли наш файл директорией, если да, то начинаем рекурсивный обход всех его файлов
 * 3) Иначе, если полученный директорией не являлся, то мы сохраняем размер файла в мапе по ключу, являющемуся расширением  файла
*/
QMap<QString, QPair<qint64, qreal>> FileGroupingByType::recursiveCalculate(QMap<QString, QPair<qint64, qreal>> &info, const QString &path, QDir::Filters filters) {
    QFileInfo file(path);

    if (file.isDir()) {
        QDir dir = file.dir();

        //Проверяем, можем ли мы зайти в директорию и не является ли она пустой
        if (dir.cd(file.fileName())) {
            if (!dir.isEmpty()) {
                /*
                Если зашли в папку, то пройдемся по контейнеру QFileInfoList, полученного методом
                entryInfoList,
                */
               foreach (QFileInfo file_info, dir.entryInfoList(filters)) {
                   //начинаем рекурсивный обход
                   info = recursiveCalculate(info, file_info.filePath(), filters);
               }
            }

            //выходим из папки
            dir.cdUp();
        } else {
            throw std::runtime_error("Can't go to directory");
        }
    } else {
        QString key;
        //если файл без расширения, то заносим его под ключ "Others"
        if (file.suffix() == "") {
            key = "Others";
        } else {
            key = "*." + file.suffix();
        }

        if (info.contains(key)) {
            info[key].first += file.size();
        } else {
            info[key].first = file.size();
            info[key].second = 0.0;
        }


        return info;
    }

    return info;
}
