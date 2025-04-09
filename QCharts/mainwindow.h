#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QChart>
#include<QChartView>
#include<QSharedPointer>
#include<QVector>
#include <QListView>
#include <QStackedWidget>
#include <QListWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void switchChart(QListWidgetItem *item);
private:
    Ui::MainWindow *ui;
    QListWidget *list;
    QStackedWidget *QStack;
    QVector<QSharedPointer<QChartView>>View;
    void setView(QChart *chart);
    void createChart_1();
    void createChart_2();
    void createChart_3();
    void createChart_4();
    void init();
    void initSignal();
};
#endif // MAINWINDOW_H
