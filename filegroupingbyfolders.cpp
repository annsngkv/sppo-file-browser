#include "filegroupingbyfolders.h"
#include <QDebug>
#include <QtMath>

/*
 * В данной функции происходит следующее:
 * 1) Вычилсяем размер файлов, сгруппировнных по папкам
 * 2) В полученную структуру заносим информацию о размере групп в процентах
 * 3) Если исходная папка была пуста, то указываем, что ее размер равен 0
*/
QList<SomeData> FileGroupingByFolders::calculateAndGroup(const QString &path, QDir::Filters filters) {
    QMap<QString, QPair<qint64, qreal>> foldersInfo;

    QFileInfo file(path);
    QString key;

    if (file.isDir()) {
        QDir dir = file.dir();

        //Проверяем, можем ли мы зайти в директорию и не является ли она пустой
        if (dir.cd(file.fileName())) {
            if (!dir.isEmpty()) {
                /*
                Если зашли в папку, то пройдемся по контейнеру QFileInfoList, полученного методом
                entryInfoList
                */
               qint64 root_size = 0;

               foreach (QFileInfo file_info, dir.entryInfoList(filters)) {
                   //начинаем рекурсивный обход
                   qint64 size = 0;

                   //Если это папка, то обходим ее рекурсивно и сохраняем полученный размер
                   if (file_info.isDir()) {
                       size = recursiveCalculate(size, file_info.filePath(), filters);

                       if (size != 0) {
                           key = file_info.fileName();
                           foldersInfo[key] = qMakePair(size, 0.0);
                       }
                   } else {//иначе суммируем размер всех файлов папки верхнего уровня
                       root_size += file_info.size();
                   }
               }

               //заносим размер папки верхнего уровня
               foldersInfo[dir.dirName()] = qMakePair(root_size, 0.0);

               setPercents(foldersInfo);
            } else {
                foldersInfo[dir.dirName()] = qMakePair(0, 0.0);
            }

           //выходим из папки
           dir.cdUp();
        } else {
            throw std::runtime_error("Can't go to directory");
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
 * 3) Иначе, если полученный директорией не являлся, то мы сохраняем размер файла в мапе по ключу, являющемуся названием папки
*/
qint64 FileGroupingByFolders::recursiveCalculate(qint64 &size, const QString &path, QDir::Filters filters) {
    QFileInfo file(path);

    if (file.isDir()) {
        QDir dir = file.dir();

        //Проверяем, можем ли мы зайти в директорию и не является ли она пустой
        if (dir.cd(file.fileName()) && !dir.isEmpty()) {
            /*
            Если зашли в папку, то пройдемся по контейнеру QFileInfoList, полученного методом
            entryInfoList,
            */
           foreach (QFileInfo file_info, dir.entryInfoList(filters)) {
               //начинаем рекурсивный обход
               size = recursiveCalculate(size, file_info.filePath(), filters);
           }

           //выходим из папки
           dir.cdUp();
        }

    } else {
        size += file.size();

        return size;
    }

    return size;
}

