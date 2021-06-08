#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include "filebrowserdatamodel.h"
#include <QAbstractItemModel>
#include "iostream"
#include "memory"
#include "grouper.h"
#include "filegroupingbyfolders.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void updateUI();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QComboBox *createGroupdBox() const;
private:
    QFileSystemModel *fileModel;
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    QTableView *tableView;
    FileBrowserDataModel *tablemodel;
    QComboBox *m_groupComboBox;
    QString currentPath = "";
    Grouper* grouper;
    QList<SomeData> *some_data;
};

#endif // MAINWINDOW_H
