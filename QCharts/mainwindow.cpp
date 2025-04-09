#include "mainwindow.h"
#include "ui_mainwindow.h"


#include<QChart>
#include <QLineSeries>
#include<QValueAxis>
#include<QSharedPointer>
#include<QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QCandlestickSeries>
#include <QCandlestickSet>
#include<QHBoxLayout>
#include<QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    createChart_1();
    createChart_2();
    createChart_3();
    createChart_4();

    //initList();
    for (const auto& chartView : View) {
        QString itemStr = chartView->chart()->title();
        qDebug() << itemStr;
        list->addItem(new QListWidgetItem(itemStr));
    }

    initSignal();  //初始化信号

}

void MainWindow::init(){
    list=new QListWidget();
    QStack=new QStackedWidget();

    QHBoxLayout * Hlayout=new QHBoxLayout();
    Hlayout->addWidget(list);
    Hlayout->addWidget(QStack);

    QVBoxLayout *Vlayout=new QVBoxLayout();
    Vlayout->addLayout(Hlayout);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    central->setLayout(Vlayout);
}


void MainWindow::initSignal(){
    connect(list, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        int index = list->row(item);            // 获取被双击项的行号
        QStack->setCurrentIndex(index);         // 切换 QStackedWidget 页面
    });

}


void MainWindow::createChart_1(){
    QChart* chart =new QChart;
    chart->setTitle("折线图");

    QLineSeries *series = new QLineSeries();
    series->append(0, 1);
    series->append(1, 3);
    series->append(2, 7);
    series->append(3, 9);
    series->append(4, 6);
    chart->addSeries(series);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 4);
    axisX->setTitleText("X 轴");

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 10);
    axisY->setTitleText("Y 轴");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    setView(chart);
}

void MainWindow::createChart_2(){
    QChart* chart =new QChart;
    chart->setTitle("饼图");

    QPieSeries* pieSeries=new QPieSeries;
    pieSeries->append(tr("男生"),17);
    pieSeries->append(tr("女生"),35);
    pieSeries->append(tr("变性人"),24);
    pieSeries->append(tr("哈基米"),38);
    pieSeries->append(tr("塑料带"),43);

    for (auto slice : pieSeries->slices()) {
        // 计算百分比
        qreal percent = slice->percentage() * 100.0;
        QString label = QString("%1: %2%").arg(slice->label()).arg(percent, 0, 'f', 1); // 保留1位小数
        slice->setLabel(label);
        slice->setLabelVisible(true);
    }

    chart->addSeries(pieSeries);

    setView(chart);
}

void MainWindow::createChart_3(){
    QChart *chart = new QChart();
    chart->setTitle("柱状图示例");

    QBarSet *set0 = new QBarSet("哈哈");
    *set0 << 1 << 2 << 3 << 4 << 5 << 6; // 添加数据

    QBarSet *set1 = new QBarSet("嘿嘿");
    *set1 << 5 << 0 << 0 << 4 << 0 << 7; // 添加数据

    QBarSet* set2=new QBarSet("啦啦");
    *set2<<1<<3<<4<<6<<7<<8;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    chart->addSeries(series);

    // 设置X轴标签（分类轴）
    QStringList categories;
    categories << "一月" << "二月" << "三月" << "四月" << "五月" << "六月";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 设置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 10);
    axisY->setTickCount(6);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setView(chart);
}

void MainWindow::createChart_4(){
    QChart *chart = new QChart();
    chart->setTitle("蜡烛图");

    // 创建蜡烛图系列
    QCandlestickSeries *series = new QCandlestickSeries();
    series->setName("K线");
    series->setIncreasingColor(Qt::red);   // 上涨颜色
    series->setDecreasingColor(Qt::green); // 下跌颜色

    // 创建蜡烛数据
    QCandlestickSet *set1 = new QCandlestickSet(10, 15, 8, 13, 1); // open, high, low, close, timestamp
    QCandlestickSet *set2 = new QCandlestickSet(13, 18, 12, 17, 2);
    QCandlestickSet *set3 = new QCandlestickSet(17, 20, 16, 16, 3);
    QCandlestickSet *set4 = new QCandlestickSet(16, 19, 14, 15, 4);

    // 添加数据到 series
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    // 添加 series 到 chart
    chart->addSeries(series);

    // 设置 X 轴为分类轴（时间戳）
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList() << "Day1" << "Day2" << "Day3" << "Day4");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 设置 Y 轴范围
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 25);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setView(chart);
}




void MainWindow::setView(QChart *chart){
    QSharedPointer<QChartView>view = QSharedPointer<QChartView>::create();
    view->setRenderHint(QPainter::Antialiasing);
    view->setChart(chart);
    QStack->addWidget(view.data());
    View.emplaceBack(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
