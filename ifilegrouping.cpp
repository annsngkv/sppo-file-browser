#include "ifilegrouping.h"
#include "QDebug"

/*
 * В данной функции мы получаем размер файла в килобайтах,
 * округляем это значение до двух знаков после запятой
 * и возвращаем строку с этим значением и суффиксом 'kb'
*/
QString IFileGrouping::getKb(qint64 num) {
    QString kb;
    kb.setNum(qreal(qRound((qreal(num) / 1024) * 100)) / 100);

    return kb += "kb";
}


qreal IFileGrouping::getPercent(qreal num, qint64 totalNum) {
    qreal percent = (qreal(num) * 100) / totalNum;

    return percent;
}

/*
 * В данной функции итерируемся по всем мапу
 * и суммируем все размеры для получения общего
*/
qint64 IFileGrouping::getTotalSize(QMap<QString, QPair<qint64, qreal>> const &map) {
    quint64 totalSize = 0;

    QMapIterator<QString, QPair<qint64, qreal>> map_it(map);

    while (map_it.hasNext()) {
        map_it.next();
        totalSize += map_it.value().first;
    }

    return totalSize;
}

/*
 * Округляем значение в процентах до 2-х знаков после зааятой и возвращаем строку с полученным значением и суффиксом "%"
 * Если размер файла в процентах меньше, чем 0.01, то будем отображать так: "<0.01%"
*/
QString IFileGrouping::getPercentWithSuffix(qreal percent) {
    qreal round_percent = qreal(qRound(percent * 100)) / 100;
    QString percent_str;

    if (percent < 0.01 && percent != 0) {
        percent_str = "<0.01";
    } else {
        percent_str.setNum(round_percent);
    }

    return percent_str += "%";
}

/*
 * Устанавливаем структуре, хранящую группы с их размерами размеры в процентах
*/
void IFileGrouping::setPercents(QMap<QString, QPair<qint64, qreal>> &files_info) {
    QMapIterator<QString, QPair<qint64, qreal>> f_it(files_info);
    qint64 totalSize = getTotalSize(files_info);

    while (f_it.hasNext()) {
        f_it.next();

        files_info[f_it.key()].second = getPercent(f_it.value().first, totalSize);
    }
}

/*
 * Отображение структуры, хранящей информацию о файлах по группам
*/
void IFileGrouping::displayFilesInfo(QList<SomeData> const &files_info) {
    QListIterator<SomeData> f_it(files_info);

    while (f_it.hasNext()) {
        SomeData sd = f_it.next();
        qDebug() << sd.name << sd.size << "/" << sd.percent;
    }
}

void IFileGrouping::attach(FileBrowserObserver* adapter) {
    adapters.push_back(adapter);
}

void IFileGrouping::detach(FileBrowserObserver* adapter) {
    adapters.erase(std::remove(adapters.begin(), adapters.end(), adapter), adapters.end());
}

void IFileGrouping::notify(const QList<SomeData> &data) {
    for (std::vector<FileBrowserObserver*>::const_iterator iter = adapters.begin(); iter != adapters.end(); ++iter) {
        if (*iter != 0) {
            (*iter)->displayUpdate(data); //каждый подписанный объект обновляет свое отображение
        }
    }
}
