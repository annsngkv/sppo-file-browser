#include <QCoreApplication>
#include <QMapIterator>
#include <QMap>
#include <QString>
#include <QPair>
#include <QDebug>
#include "filegroupingbyfolders.h"
#include "filegroupingbytype.h"
#include "ifilegrouping.h"
#include "grouper.h"
#include "iostream"
#include "memory"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string path;
    std::string strategy;

    /*
     * QDir::Files  - List files
     * QDir::NoDotAndDotDot - Do not list the special entries "." and "..".
     * QDir::NoSymLinks - Do not list symbolic links (ignored by operating systems that don't support symbolic links).
     * QDir::Hidden - List hidden files (on Unix, files starting with a ".").
     * QDir::Dirs - List directories that match the filters
    */
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden  | QDir::Dirs;

    /*
     * Консольный интерфейс для клиента:
     * необходимо выбрать путь(полный) до папки
     * и выбрать споосб группировки
    */
    std::cout << "Ведите полный путь до папки" << std::endl;
    std::cin >> path;
    std::cout << "Выберите способ группировки" << std::endl << "По папкам: 0" << std::endl << "По типу фалов: 1" << std::endl;
    std::cin >> strategy;

    if (!std::stoi(strategy)) {
        Grouper group_by_folders(std::make_shared<FileGroupingByFolders>());
        group_by_folders.grouping(QString(path.c_str()), filters);
    } else {
        Grouper group_by_type(std::make_shared<FileGroupingByType>());
        group_by_type.grouping(QString(path.c_str()), filters);
    }

    return a.exec();
}
