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
#include "filegroupingbytype.h"
#include "piechartadapter.h"
#include "tableviewadapter.h"
#include <QSplitter>
#include "barchartadapter.h"

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void updateGrouping();
    void updateDisplay();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QComboBox *createGroupBox() const;
    QComboBox *createDisplayBox() const;
private:
    QFileSystemModel *fileModel;
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    QTableView *tableView;
    QChartView *pieChartView;
    QChartView *barChartView;
    FileBrowserDataModel *tablemodel;
    QComboBox *m_groupComboBox;
    QComboBox *m_displayComboBox;
    QString currentPath = "";
    Grouper* grouper;
    QList<SomeData> *some_data;
    FileGroupingByFolders *fileGroupingByFolders;
    FileGroupingByType *fileGroupingByType;
    PieChartAdapter *pieChartAdapter;
    BarChartAdapter *barChartAdapter;
    TableViewAdapter *tableViewAdapter;
    QSplitter *splitter;
};

#endif // MAINWINDOW_H
