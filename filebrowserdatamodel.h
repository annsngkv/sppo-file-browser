#ifndef FILEBROWSERDATAMODEL_H
#define FILEBROWSERDATAMODEL_H

#include <QObject>

class FileBrowserDataModel : public QObject
{
    Q_OBJECT
public:
    explicit FileBrowserDataModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FILEBROWSERDATAMODEL_H