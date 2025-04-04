#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include<QListWidget>
#include<QMediaDevices>

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
    void on_addBtn_clicked();

    void on_removeBtn_clicked();

    void on_clearBtn_clicked();

    void on_conBtn_clicked();

    void on_pauseBtn_clicked();

    void on_stopBtn_clicked();

    void on_preBtn_clicked();

    void on_nextBtn_clicked();

private:
    Ui::MainWindow *ui;

    QMediaPlayer * player;
    QListWidget* playerList;
    QAudioOutput* audio;   //输出播放

    int index;
    void init();
    void initSignal();
};
#endif // MAINWINDOW_H
