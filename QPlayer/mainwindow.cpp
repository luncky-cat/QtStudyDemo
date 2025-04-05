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

    sliderIsPressed=false;

    QAudioDevice device = QMediaDevices::defaultAudioOutput();
    audio = new QAudioOutput(device, this);
    audio->setVolume(0.1);  // 设置音量

    player = new QMediaPlayer(this);
    player->setAudioOutput(audio);

    ui->soundSlider->setRange(0,100);
    ui->soundSlider->setValue(audio->volume()*100);  //当前音量
    ui->vrateLab->setText(QString("%1%").arg(audio->volume()*100));

    ui->songProSlider->setRange(0,100);

    index=-1;

}

QString MainWindow::getTime(qint64 value) {
    int sec = value / 1000;
    int min = sec / 60;
    sec = sec % 60; // 保留分钟后的秒数

    return QString("%1:%2")
        .arg(min, 2, 10, QChar('0'))
        .arg(sec, 2, 10, QChar('0'));
}




void MainWindow::startplayer(QListWidgetItem *item){
    if(!player->isAvailable()){
        return;
    }

    filePath = item->data(Qt::UserRole).toString();
    this->player->setSource(QUrl::fromLocalFile(filePath));
    this->player->play();
    index=playerList->currentRow();
}

void MainWindow::updatePlayerInfo(QMediaPlayer::MediaStatus statue){
    if(statue==QMediaPlayer::MediaStatus::LoadedMedia){
        ui->nowTimeLab->setText(getTime(player->duration()));
    }
}

void MainWindow::updateSlider(qint64 value) {
    if (sliderIsPressed) {
        return; // 拖动时，播放器更新忽略
    }

    int v = value * 100 / player->duration();
    ui->startTimeLab->setText(getTime(value));
    ui->songProSlider->setSliderPosition(v); // 仅在非拖动时更新
}


void MainWindow::initSignal(){
    connect(playerList, &QListWidget::itemDoubleClicked, this, &MainWindow::startplayer);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSlider);

    connect(ui->songProSlider, &QSlider::sliderPressed, this, [=]() {
        sliderIsPressed = true;
    });

    connect(ui->songProSlider, &QSlider::sliderMoved, this, [=](int pos) {
        qint64 duration = player->duration();
        qint64 newPos = static_cast<qint64>(pos) * duration / 100; // 转换为实际时间位置
        player->setPosition(newPos);
    });

    connect(ui->songProSlider, &QSlider::sliderReleased, this, [=]() {
        sliderIsPressed = false;
    });

    connect(ui->soundSlider, &QSlider::valueChanged, [&](int value) {
        player->audioOutput()->setVolume(value/100.0);
        ui->vrateLab->setText(QString("%1%").arg(value));
    });

    connect(player,&QMediaPlayer::mediaStatusChanged,this,&MainWindow::updatePlayerInfo);   //加载好更新歌曲信息

    connect(player,&QMediaPlayer::sourceChanged,this,[=](){     //设置源更新
          ui->songNameLab->setText(QFileInfo(filePath).baseName());
          ui->startTimeLab->setText("00:00");
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
        ui->playerList->setCurrentRow(index);
    }
}


void MainWindow::on_removeBtn_clicked()
{
    QListWidgetItem*item=playerList->currentItem();
    int curr=playerList->currentRow();
    if(item){
        if(index==curr){
            while(!on_nextBtn_clicked());
        }
        delete item;
    }
}


void MainWindow::on_clearBtn_clicked()
{
    if (player->playbackState() != QMediaPlayer::StoppedState) {
        player->stop();
    }
    playerList->clear();  //清空
}


void MainWindow::on_conBtn_clicked()
{
    if(player->playbackState()==QMediaPlayer::PausedState){
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


QListWidgetItem * MainWindow::on_preBtn_clicked()   //上一首
{
    int pre = index - 1;

    if (pre < 0) {
        pre = playerList->count() - 1;
    }

    QListWidgetItem *item = playerList->item(pre);

    if (item) {
        index = pre;
        playerList->setCurrentRow(index);
        emit playerList->itemDoubleClicked(item);
        return item;
    }
    return nullptr;
}



QListWidgetItem * MainWindow::on_nextBtn_clicked()     //下一首
{
    int next=index+1;

    if(next>=playerList->count()){
        next=0;
    }

    QListWidgetItem *item = playerList->item(next);

    if (item) {
        index = next;
        playerList->setCurrentRow(index);
        emit playerList->itemDoubleClicked(item);
        return item;
    }
    return nullptr;
}

