#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdatetime.h"
#include <QAudioDevice>
#include <QAudioInput>
#include <QCamera>
#include <QImageCapture>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QVideoWidget>

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
    void adjustLayout();  // 用于重新应用原始布局

    void on_openAct_triggered(bool checked);

    void on_stopRAtc_triggered(bool checked);

    void on_startRAct_triggered(bool checked);
    void on_touchAct_triggered();
    void on_closeAct_triggered(bool checked);
    void on_exitAct_triggered();
    void on_devicesCBx_currentIndexChanged(int index);
    void on_audioCBx_currentIndexChanged(int index);
    void on_checkBox_3_checkStateChanged(const Qt::CheckState &arg1);
    void updateActions();
    void on_chosesPathBtn_clicked();

private:
    Ui::MainWindow *ui;

    void updateRtiem();


    QList<QAudioDevice>Audios; //可用列表
    QList<QCameraDevice>Camers;

    int cameraIndex;
    int audioIndex;

    QSharedPointer<QMediaCaptureSession>captureSession;
    QSharedPointer<QCamera> Camera;       // 相机
    QSharedPointer<QAudioInput> audioInput; // 音频输入
    QSharedPointer<QImageCapture> capture;  // 抓图
    QSharedPointer<QMediaRecorder> recorder; // 录像
    QSharedPointer<QTimer>Rtime;

    QVideoWidget* outW;


    int timeSeq;    //时间

    QString videoOutPutDir;   //视频保存文件夹
    QString videoFilePath;    //视频保存路径

    QString imageOutPutDir;  //抓图文件夹
    QString imageFilePath;   //抓图保存路径
    void init();
    void initDevice();
    void initSignals();
    void setPreview(const QImage &preview);

    void qdebug();
};
#endif // MAINWINDOW_H
