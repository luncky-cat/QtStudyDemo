#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMediaDevices>
#include<QMediaPlayer>
#include<QAudio>
#include<QAudioDevice>
#include <QAudioOutput>
#include<QFileDialog>
#include <QMediaPlayer>
#include <QAudioSink>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    initSignal();
}

void MainWindow::init(){
    ui->vmain->setSpacing(0);
    ui->vmain->setStretch(0,8);
    ui->vmain->setStretch(1,1);
    ui->vmain->setStretch(2,1);


    this->timeEndLab=ui->timeEndLab;
    this->viedoW=ui->videoW;
    this->fileNameLab=ui->fileNameLab;
    this->startTimeLab=ui->startTimeLab;
    this->nowTimeSlider=ui->timeSlider;
    this->soundSlider=ui->soundSlider;
    this->soundLab=ui->soundLab;

    soundSlider->setRange(0,100);
    nowTimeSlider->setRange(0,100);

    audioOutput = new QAudioOutput(QMediaDevices::defaultAudioOutput());
    audioOutput->setVolume(0.1);
    player = new QMediaPlayer();
    player->setAudioOutput(audioOutput);
    player->setVideoOutput(ui->videoW);
}


void MainWindow::timeSilderPositionChanged(qint64 value){   //时间滑条更新
    qint64 duration = player->duration();
    if (duration > 0) {
        auto progress = static_cast<double>(value) / duration;
        nowTimeSlider->setSliderPosition(static_cast<int>(progress * 100));  // 滑条0-100
        int seconds = value / 1000;
        int minutes = seconds / 60;
        seconds %= 60;
        startTimeLab->setText(QString("%1:%2")
                                      .arg(minutes, 2, 10, QLatin1Char('0'))
                                      .arg(seconds, 2, 10, QLatin1Char('0')));
    }
}

void MainWindow::loadMedia(QMediaPlayer::MediaStatus status){
    if (status == QMediaPlayer::LoadedMedia) {
        qint64 duration = player->duration();
        int seconds = duration / 1000;
        int minutes = seconds / 60;
        seconds %= 60;
        timeEndLab->setText(QString("%1:%2")
                                    .arg(minutes, 2, 10, QLatin1Char('0'))
                                    .arg(seconds, 2, 10, QLatin1Char('0')));
    }
}


void MainWindow::initSignal(){
    connect(soundSlider, &QSlider::valueChanged, this, [=](int value) {   //滑条变音量
        audioOutput->setVolume(value / 100.0);
        soundLab->setText(QString("%1%").arg(value));
    });

    connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::timeSilderPositionChanged);   //播放

    connect(nowTimeSlider, &QSlider::sliderPressed, this, [=]() {    //拖住
        disconnect(player,&QMediaPlayer::positionChanged, this,&MainWindow::timeSilderPositionChanged);
    });

    connect(nowTimeSlider, &QSlider::sliderReleased, this, [=]() {  //释放
        connect(player, &QMediaPlayer::positionChanged, this,&MainWindow::timeSilderPositionChanged);
    });


    connect(nowTimeSlider, &QSlider::sliderMoved, this, [=](int position) {   // 拖动滑块时设置播放进度
        player->setPosition(position * player->duration() / 100);
    });

    connect(player,&QMediaPlayer::sourceChanged,this,&MainWindow::updateSrcNew);

    connect(player, &QMediaPlayer::mediaStatusChanged, this,&MainWindow::loadMedia);

    connect(player, &QMediaPlayer::playbackStateChanged,this, [=](QMediaPlayer::PlaybackState state) {
        if(player->duration()!=player->position()){
            return;
        }
        if (state == QMediaPlayer::StoppedState) {
            player->setPosition(0);
            player->play();
        }
    });

}

void MainWindow::updateSrcNew(){    //更新播放源相关的界面信息
    if(player->isPlaying()){
        player->stop();
    }

    timeEndLab->setText("");
    auto v=audioOutput->volume()*100;
    soundLab->setText(QString("%1%").arg(v));
    soundSlider->setSliderPosition(v);
    fileNameLab->setText(QFileInfo(openFile).baseName());
    startTimeLab->setText("00:00");
    nowTimeSlider->setSliderPosition(0);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openBtn_clicked()
{

    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "选择视频文件",
                                                    "",
                                                    "所有文件 (*.*);;"
                                                    "视频文件 (*.mp4 *.avi *.mkv *.mov *.flv *.wmv *.webm *.mpeg *.mpg *.3gp *.rmvb *.ts)"
                                                    );
    if(filePath.isEmpty()){
        return;
    }

    openFile=filePath;
    player->setSource(QUrl::fromLocalFile(filePath));
    player->play();
}


void MainWindow::on_pauseBtn_clicked()
{
    if(player->isPlaying()){
        player->pause();
    }
}

void MainWindow::on_conBtn_clicked()
{
    if (player->playbackState() == QMediaPlayer::PausedState) {
        player->play();
    }
}

void MainWindow::on_stopBtn_clicked()
{
    if(player->isPlaying()||player->playbackState()==QMediaPlayer::PausedState){
        player->stop();
        updateSrcNew();
    }
}


void MainWindow::on_allShowBtn_clicked()
{
    viedoW->setFullScreen(true);
}

