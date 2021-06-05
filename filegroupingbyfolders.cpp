#include "filegroupingbyfolders.h"
#include <QDebug>
#include <QtMath>

/*
 * В данной функции происходит следующее:
 * 1) Вычилсяем размер файлов, сгруппировнных по папкам
 * 2) В полученную структуру заносим информацию о размере групп в процентах
 * 3) Если исходная папка была пуста, то указываем, что ее размер равен 0
*/
QMap<QString, QPair<qint64, qreal>> FileGroupingByFolders::calculateAndGroup(const QString &path, QDir::Filters filters) {
    QMap<QString, QPair<qint64, qreal>> foldersInfo;

    foldersInfo = recursiveCalculate(foldersInfo, path, filters);
    setPercents(foldersInfo);

    qint64 total_size = getTotalSize(foldersInfo);

    foldersInfo["total_size"].first = total_size;

    if (total_size == 0) {
        foldersInfo["total_size"].second = 0.0;
    }

    return foldersInfo;
}

/*
 * Рекурсивная функция для группирования фалов по папкам и вычиления размера этой группы
 * В данной функции происходит следующее:
 * 1) Получем файл по указанному пути
 * 2) Проверяем, является ли наш файл директорией, если да, то начинаем рекурсивный обход всех его файлов
 * 3) Иначе, если полученный директорией не являлся, то мы сохраняем размер файла в мапе по ключу, являющемуся названием папки
*/
QMap<QString, QPair<qint64, qreal>> FileGroupingByFolders::recursiveCalculate(QMap<QString, QPair<qint64, qreal>> &info, const QString &path, QDir::Filters filters) {
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
               info = recursiveCalculate(info, file_info.filePath(), filters);
           }

           //выходим из папки
           dir.cdUp();
        }

    } else {
        //Если такой ключ сущестует, то увеличиваем размер
        if (info.contains(file.dir().dirName())) {
            info[file.dir().dirName()].first += file.size();
        } else {
            info[file.dir().dirName()].first = file.size();
            info[file.dir().dirName()].second = 0.0;
        }

        return info;
    }

    return info;
}

