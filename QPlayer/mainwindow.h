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

    QListWidgetItem * on_preBtn_clicked();

    QListWidgetItem * on_nextBtn_clicked();

    void startplayer(QListWidgetItem *item);

    void updatePlayerInfo(QMediaPlayer::MediaStatus statue);
    void updateSlider(qint64 value);
private:
    Ui::MainWindow *ui;
    bool sliderIsPressed;
    QMediaPlayer * player;
    QListWidget* playerList;
    QAudioOutput* audio;   //输出播放
    QString filePath;
    int index;
    void init();
    void initSignal();
    QString getTime(qint64 value);
};
#endif // MAINWINDOW_H
