#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include "grouper.h"
#include "ifilegrouping.h"
#include "filegroupingbyfolders.h"
#include "filegroupingbytype.h"
#include <QList>
#include "somedata.h"
#include <QComboBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_groupComboBox(createGroupdBox())
{
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1000, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Choosen Path: ");
    //Корневая папка
    QString homePath = QDir::homePath() + "/test_folder";

    // Определим модель файловой системы:
    dirModel =  new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath(homePath);

    //Создаем объект класса-контекста, по умолчанию группируем по папкам
    grouper = new Grouper(std::make_shared<FileGroupingByFolders>());
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden  | QDir::Dirs;
    QList<SomeData> sd = grouper->grouping(homePath, filters);

    //Пользовательская модель данных
    tablemodel = new FileBrowserDataModel(this, sd);

    //Показать как дерево, пользуясь готовым видом:
    treeView = new QTreeView();
    treeView->setModel(dirModel);
    //Раскрываем дерево полностью
    treeView->expandAll();
    treeView->header()->resizeSection(0, 200);

    //Разделитель
    QSplitter *splitter = new QSplitter(parent);
    //Контейнер для фильтра выбора типа группировки файлов
    QHBoxLayout *settingsLayout = new QHBoxLayout(parent);
    settingsLayout->addWidget(new QLabel("Group by:"));
    settingsLayout->addWidget(m_groupComboBox);

    //Показываем нашу модель в табличном виде
    tableView = new QTableView;
    tableView->setModel(tablemodel);

    //Добавляем вьюшки в сплиттер и устанавиливаем его в центре
    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    setCentralWidget(splitter);

    //Добавляем наш дроб-даун в меню окна
    QWidget *widget = new QWidget(parent);
    widget->setLayout(settingsLayout);
    setMenuWidget(widget);

    //Модель выбора
    QItemSelectionModel *selectionModel = treeView->selectionModel();

    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    //Выполняем соединение слота и сигнала, когда мы выбрали в фильтре, как группировать данные
    connect(m_groupComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateUI);

    //Пример организации установки курсора в TreeView относит ельно модельного индекса
    QItemSelection toggleSelection;
    QModelIndex topLeft;
    topLeft = dirModel->index(homePath);
    dirModel->setRootPath(homePath);

    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

//Слот для обработки выбора элемента в TreeView
//выбор осуществляется с помощью курсора
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = dirModel->filePath(ix);
        currentPath = dirModel->filePath(indexs.constFirst());
        this->statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    }

    //Показываем файлы выбранной папки в зависимости от текущего фильтра
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden  | QDir::Dirs;
    QList<SomeData> some_data;

    int idx = m_groupComboBox->itemData(m_groupComboBox->currentIndex()).toInt();

    switch (idx) {
        case 1:
            grouper->changeStrategy(std::make_shared<FileGroupingByType>());
            some_data = grouper->grouping(currentPath, filters);
            break;
        default:
            grouper->changeStrategy(std::make_shared<FileGroupingByFolders>());
            some_data = grouper->grouping(currentPath, filters);
            break;
    }

    tablemodel->needReset();
    tablemodel->setSomeData(some_data);
    tablemodel->insertRow(tablemodel->rowCount(tablemodel->index(0, 0, QModelIndex())));
    tableView->setRootIndex(tablemodel->index(0, 0, QModelIndex()));
}

MainWindow::~MainWindow()
{
    delete fileModel;
    delete dirModel;
    delete tablemodel;
    delete grouper;
    delete some_data;

}

//Создание фильтра
QComboBox* MainWindow::createGroupdBox() const
{
    QComboBox *comboBox = new QComboBox();
    comboBox->addItem("folders ", 0);
    comboBox->addItem("types", 1);

    return comboBox;
}

//Показываем файлы текущей папки в  зависимости от выбранного фильтра
void MainWindow::updateUI() {
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden  | QDir::Dirs;
    QList<SomeData> some_data;

    int idx = m_groupComboBox->itemData(m_groupComboBox->currentIndex()).toInt();

    switch (idx) {
        case 1:
            grouper->changeStrategy(std::make_shared<FileGroupingByType>());
            some_data = grouper->grouping(currentPath, filters);
            break;
        default:
            grouper->changeStrategy(std::make_shared<FileGroupingByFolders>());
            some_data = grouper->grouping(currentPath, filters);
            break;
    }

    tablemodel->needReset();
    tablemodel->setSomeData(some_data);
    tablemodel->insertRow(tablemodel->rowCount(tablemodel->index(0, 0, QModelIndex())));
    tableView->setRootIndex(tablemodel->index(0, 0, QModelIndex()));
}
