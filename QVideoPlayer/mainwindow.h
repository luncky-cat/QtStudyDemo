#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioDevice>
#include <QAudioOutput>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSlider>
#include "videoview.h"

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
    videoview* viedoW;
    QAudioDevice device;
    QAudioOutput *audioOutput;  // 需要传入设备参数
    QMediaPlayer* player;

    QLabel *fileNameLab;
    QLabel *startTimeLab;
    QLabel *timeEndLab;
    QLabel *soundLab;
    QSlider *nowTimeSlider;
    QSlider* soundSlider;
    QString openFile;   //播放的文件
protected:
    //void mouseDoubleClickEvent(QMouseEvent *event);
private slots:
    void on_openBtn_clicked();

    void on_pauseBtn_clicked();

    void on_conBtn_clicked();

    void on_stopBtn_clicked();

    void updateSrcNew();

    void timeSilderPositionChanged(qint64 value);
    void loadMedia(QMediaPlayer::MediaStatus status);
    void on_allShowBtn_clicked();

private:
    Ui::MainWindow *ui;
    void init();
    void load();
    void initSignal();

};
#endif // MAINWINDOW_H
