#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMediaPlayer>
#include<QFileDialog>
#include<QListWidgetItem>
#include<QAudioDevice>
#include<QMediaDevices>
#include <QAudioOutput>
#include <QMediaFormat>
#include <QSoundEffect>
#include <QMediaCaptureSession>


void MainWindow::init(){
    this->playerList = ui->playerList;

    QAudioDevice device = QMediaDevices::defaultAudioOutput();
    qDebug()<<device.description();


    audio = new QAudioOutput(device, this);
    audio->setVolume(50);  // 设置音量

    player = new QMediaPlayer(this);

    player->setAudioOutput(audio);

    ui->soundSlider->setRange(0,100);

    ui->soundSlider->setValue(audio->volume());  //当前音量

    ui->vrateLab->setText(QString("%1%").arg(audio->volume()));

    index=-1;

}




void MainWindow::initSignal(){
    connect(playerList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        if(player->isPlaying()){
            player->stop();
        }
        qDebug()<<player->isAvailable();
        QString filePath = item->data(Qt::UserRole).toString();
        qDebug()<<"播放音乐"<<filePath;
        this->player->setSource(QUrl::fromLocalFile(filePath));
        this->player->play();  // 播放选中的音频文件
        ui->songNameLab->setText(QFileInfo(filePath).baseName());
        ui->nowTimeLab->setText(QString::number(player->duration()));
        index=playerList->currentRow();
    });


    // 媒体时长变化，更新滑条范围
    QObject::connect(player, &QMediaPlayer::durationChanged, [&](qint64 duration) {
        ui->songProSlider->setRange(0, duration);
    });

    // 用户手动拖动滑条，改变播放进度
    QObject::connect(ui->songProSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    connect(ui->soundSlider, &QSlider::valueChanged, [&](int value) {
        player->audioOutput()->setVolume(value);
        qDebug()<<value;
        ui->vrateLab->setText(QString("%1%").arg(value));
    });
}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    initSignal();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addBtn_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
        nullptr,
        "选择文件路径",
        "/",
        "音频文件 (*.mp3);;所有文件 (*.*)"
        );

    foreach (QString file, fileList) {
        QListWidgetItem* item=new QListWidgetItem();
        item->setData(Qt::UserRole,file);
        item->setText(QFileInfo(file).fileName());
        playerList->addItem(item);
    }
    if(index<0){
        index=0;
    }
}


void MainWindow::on_removeBtn_clicked()
{
    QListWidgetItem*item=playerList->currentItem();
    if(item){
        delete item;
    }
}


void MainWindow::on_clearBtn_clicked()
{
    playerList->clear();  //清空
}


void MainWindow::on_conBtn_clicked()
{
    if(player->playbackRate()==QMediaPlayer::PausedState){
        player->play();
    }
}


void MainWindow::on_pauseBtn_clicked()
{
    if(player->isPlaying()){
        player->pause();
    }
}


void MainWindow::on_stopBtn_clicked()
{
    player->stop();
}


void MainWindow::on_preBtn_clicked()
{
    int pre = index - 1;

    if (pre < 0) {
        pre = playerList->count() - 1;
    }

    QListWidgetItem *item = playerList->item(pre);

    if (item) {
        index = pre;
        emit playerList->itemDoubleClicked(item);
    }
}



void MainWindow::on_nextBtn_clicked()
{
    int next=index+1;

    if(next>=playerList->count()){
        next=0;
    }

    QListWidgetItem *item = playerList->item(next);

    if (item) {
        index = next;
        emit playerList->itemDoubleClicked(item);
    }

}

