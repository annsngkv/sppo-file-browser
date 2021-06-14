#include "mainwindow.h"
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
      m_groupComboBox(createGroupBox()),
      m_displayComboBox(createDisplayBox())
{
    //Определям стратегии
    fileGroupingByFolders = new FileGroupingByFolders();
    fileGroupingByType = new FileGroupingByType();
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
    grouper = new Grouper(fileGroupingByFolders);
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
    splitter = new QSplitter(parent);
    //Контейнер для фильтра выбора типа группировки файлов и типа отображения группировки
    QHBoxLayout *settingsLayout = new QHBoxLayout(parent);
    settingsLayout->addWidget(new QLabel("Group by:"));
    settingsLayout->addWidget(m_groupComboBox);
    settingsLayout->addWidget(new QLabel("Display in:"));
    settingsLayout->addWidget(m_displayComboBox);

    //Показываем нашу модель в табличном виде
    tableView = new QTableView;
    tableView->setModel(tablemodel);

    //Определяем вьюхи для отображения диаграмм
    pieChartView = new QChartView();
    barChartView = new QChartView();
    //Опредеялем адаптеры
    pieChartAdapter = new PieChartAdapter(pieChartView);
    barChartAdapter = new BarChartAdapter(barChartView);
    tableViewAdapter = new TableViewAdapter(tableView, tablemodel);
    //Подписываем адаптеры-наблюдатели на стратегии
    fileGroupingByFolders->attach(pieChartAdapter);
    fileGroupingByType->attach(pieChartAdapter);
    fileGroupingByFolders->attach(barChartAdapter);
    fileGroupingByType->attach(barChartAdapter);
    fileGroupingByFolders->attach(tableViewAdapter);
    fileGroupingByType->attach(tableViewAdapter);
    //Добавляем вьюшки в сплиттер и устанавиливаем его в центре (по умолчанию отображаем в табличном виде
    //и группируем по папкам)
    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    setCentralWidget(splitter);

    //Добавляем наши дроб-даун в меню окна
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
            this, &MainWindow::updateGrouping);
    //Выполняем соединение слота и сигнала, когда мы выбрали в фильтре, как отображать
    //сгруппированные данные
    connect(m_displayComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateDisplay);

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
            grouper->changeStrategy(fileGroupingByType);
            some_data = grouper->grouping(currentPath, filters);
            break;
        default:
            grouper->changeStrategy(fileGroupingByFolders);
            some_data = grouper->grouping(currentPath, filters);
            break;
    }
}

MainWindow::~MainWindow()
{
    delete fileModel;
    delete dirModel;
    delete tablemodel;
    delete grouper;
    delete some_data;
    delete splitter;
    delete treeView;
    delete tableView;
    delete pieChartView;
    delete barChartView;
    delete m_groupComboBox;
    delete m_displayComboBox;
    delete fileGroupingByFolders;
    delete fileGroupingByType;
    delete pieChartAdapter;
    delete barChartAdapter;
    delete tableViewAdapter;
    delete splitter;
}

//Создание фильтра для выбора группирования файлов
QComboBox* MainWindow::createGroupBox() const
{
    QComboBox *comboBox = new QComboBox();
    comboBox->addItem("folders ", 0);
    comboBox->addItem("types", 1);

    return comboBox;
}

//Создание фильтра для выбора типа отображения сгруппированный файлов
QComboBox* MainWindow::createDisplayBox() const
{
    QComboBox *comboBox = new QComboBox();
    comboBox->addItem("Table View", 0);
    comboBox->addItem("Pie Chart", 1);
    comboBox->addItem("Bar Chart", 2);

    return comboBox;
}

//Показываем файлы текущей папки в  зависимости от выбранного фильтра
void MainWindow::updateGrouping() {
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden  | QDir::Dirs;
    QList<SomeData> some_data;

    int idx = m_groupComboBox->itemData(m_groupComboBox->currentIndex()).toInt();

    switch (idx) {
        case 1:
            grouper->changeStrategy(fileGroupingByType);
            some_data = grouper->grouping(currentPath, filters);
            break;
        default:
            grouper->changeStrategy(fileGroupingByFolders);
            some_data = grouper->grouping(currentPath, filters);
            break;
    }

}

//Показываем сгрупированные файлы в различных видах, в зависимости от выбранного фильтра
void MainWindow::updateDisplay() {
    int idx = m_displayComboBox->itemData(m_displayComboBox->currentIndex()).toInt();

    switch (idx) {
        case 1:
            splitter->replaceWidget(1, pieChartView);
            break;
        case 2:
            splitter->replaceWidget(1, barChartView);
            break;
        default:
            splitter->replaceWidget(1, tableView);
            break;
    }
}
