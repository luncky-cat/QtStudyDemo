#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QVideoWidget>
#include<QCameraFormat>
#include <QTimer>
#include <Qtime>
#include <QFileInfo>
#include<QFileDialog>


void MainWindow::initDevice(){

    Audios=QMediaDevices::audioInputs();
    Camers=QMediaDevices::videoInputs();
}

void MainWindow::init(){
    videoOutPutDir="D:/Documents/QCameraDemo/";
    videoFilePath.clear();

    imageOutPutDir="D:/Documents/";
    imageFilePath.clear();

    timeSeq=1;

    captureSession = QSharedPointer<QMediaCaptureSession>::create();

    Camera = QSharedPointer<QCamera>::create();        // 创建相机

    audioInput = QSharedPointer<QAudioInput>::create();// 创建音频输入

    capture = QSharedPointer<QImageCapture>::create(); // 绑定相机

    recorder = QSharedPointer<QMediaRecorder>::create(); // 创建录像

    Rtime = QSharedPointer<QTimer>::create(); // 创建录像

    capture->setFileFormat(QImageCapture::FileFormat::PNG);  //设置格式
    capture->setQuality(QImageCapture::Quality::VeryLowQuality);
    captureSession->setImageCapture(capture.data());

    ui->cameraGBox->setMaximumHeight(120);
    ui->defalutPathLabel->setText(QString("默认保存目录:%1").arg(videoOutPutDir));

    for (const QAudioDevice &device : Audios) {
        ui->audioCBx->addItem(device.description());
    }

    for(const QCameraDevice &device:Camers){
        ui->devicesCBx->addItem(device.description());
    }

    cameraIndex=ui->devicesCBx->currentIndex();
    audioIndex=ui->audioCBx->currentIndex();


    outW = new QVideoWidget(ui->groupBox_2);
    ui->groupBox_2->layout()->addWidget(outW);

    QList<QAction*> allActions = this->findChildren<QAction*>();
    for (QAction* act : allActions) {
        act->setEnabled(false);  // 或 act->setDisabled(true);
    }
    ui->openAct->setEnabled(true);

    ui->statueLab->setAlignment(Qt::Alignment::enum_type::AlignLeft);

}

void MainWindow::on_devicesCBx_currentIndexChanged(int index)
{
    this->cameraIndex=index;
}

void MainWindow::on_audioCBx_currentIndexChanged(int index)
{
    this->audioIndex=index;
}

void MainWindow::initSignals(){
    connect(capture.data(), &QImageCapture::imageCaptured, this, [=](int id, const QImage &preview) {setPreview(preview);});
    connect(Rtime.data(),&QTimer::timeout,this,&MainWindow::updateRtiem);
    connect(recorder.data(),&QMediaRecorder::recorderStateChanged,this,&MainWindow::updateActions);
}

void  MainWindow::setPreview(const QImage &preview){
    QPixmap pixmap = QPixmap::fromImage(preview).scaled(ui->captrueLab->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->fileSavePatLab->setText(QString("文件名:%1").arg(QFileInfo(imageFilePath).fileName()));
    ui->captrueLab->setPixmap(pixmap);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDevice();
    init();
    adjustLayout();
    initSignals();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateActions()
{

    bool isCameraOpen = Camera && Camera->isActive(); // 相机开启
    bool isRecording = recorder&&(recorder->recorderState()==QMediaRecorder::RecordingState); // 录像
    // 控制打开/关闭
    ui->openAct->setEnabled(!isCameraOpen);  //相机未开
    ui->closeAct->setEnabled(isCameraOpen&&!isRecording);   //打开相机且不在录像
    // 控制抓拍和录像
    ui->touchAct->setEnabled(isCameraOpen); // 相机开启抓拍
    ui->startRAct->setEnabled(isCameraOpen && !isRecording); // 录像没开始时可以开始录像
    ui->stopRAtc->setEnabled(isCameraOpen&&isRecording); // 不在录像时可以关闭
    // 退出可以一直用
    ui->exitAct->setEnabled(true);
}


void MainWindow::adjustLayout()
{
    ui->V_main->setSpacing(0);
    ui->V_main->setStretch(0,1);
    ui->V_main->setStretch(1,1);
    ui->H_main->setSpacing(0);
    ui->H_main->setStretch(0,1);
    ui->H_main->setStretch(1,1);
    ui->V_mainL->setSpacing(0);
    ui->V_mainL->setStretch(0,9);
    ui->V_mainL->setStretch(1,1);
    ui->V_mainL_1->setSpacing(0);
    ui->V_mainL_1->setContentsMargins(0,0,0,0);
    ui->V_mainR->setSpacing(0);
    ui->V_mainR->setStretch(0,9);
    ui->V_mainR->setStretch(1,1);
    ui->V_mainR_1->setSpacing(0);
    ui->V_mainR_1->setSpacing(0);
    ui->groupBox_2->setContentsMargins(10,10,10,10);
    ui->groupBox_3->setContentsMargins(10,10,10,10);
}

void MainWindow::updateRtiem(){
    ui->RtimeLabel->setText(QString("已经录制%1秒").arg(timeSeq++));
}

void MainWindow::on_checkBox_3_checkStateChanged(const Qt::CheckState &arg1)
{
    if(Qt::Checked==arg1){
        captureSession->setAudioInput(nullptr);
        return;
    }
    if(!captureSession.isNull()){
         captureSession->setAudioInput(audioInput.data());
    }
}


void MainWindow::on_openAct_triggered(bool checked)
{
    Camera =QSharedPointer<QCamera>::create(Camers[cameraIndex]);
    audioInput=QSharedPointer<QAudioInput>::create(Audios[cameraIndex]);
    QList<QCameraFormat> formats = Camers[cameraIndex].videoFormats();
    QSize resolution = formats.first().resolution();
    int currentWidth = outW->width();
    int currentHeight = outW->height();
    qreal xR = (qreal)resolution.width() / currentWidth;
    qreal yR = (qreal)resolution.height() / currentHeight;
    qreal scale = qMax(xR, yR);
    int newWidth = resolution.width() / scale;
    int newHeight = resolution.height() / scale;
    outW->setMinimumSize(newWidth, newHeight);
    outW->setFixedSize(newWidth, newHeight);
    connect(Camera.data(),&QCamera::activeChanged,this,[=](){
        ui->statueLab->setText(QString("摄像头状态：%1").arg(Camera->isActive()?"Active":"Stoped"));
        updateActions();
    });

    captureSession->setCamera(Camera.data());
    captureSession->setAudioInput(audioInput.data());
    captureSession->setVideoOutput(outW);
    Camera->start();
}


void MainWindow::on_stopRAtc_triggered(bool checked)
{
    Rtime->stop();
    timeSeq=1;
    recorder->stop();
    captureSession->setVideoOutput(nullptr);  // 取消绑定视频
    outW->close();
    outW=new QVideoWidget(ui->groupBox_2);
    captureSession->setVideoOutput(outW);
    ui->groupBox_2->layout()->addWidget(outW);
}

void MainWindow::on_startRAct_triggered(bool checked)
{
    captureSession->setRecorder(recorder.data());
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    videoFilePath = videoOutPutDir + "Video_" + timestamp + ".avi";
    recorder->setOutputLocation(QUrl::fromLocalFile(videoFilePath));
    recorder->record();
    Rtime->start(1000);
}


void MainWindow::on_touchAct_triggered()    //抓图
{
    imageFilePath.clear();
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    imageFilePath = imageOutPutDir+ "captrue_" + timestamp + ".png";
    qDebug()<<imageFilePath;
    capture->captureToFile(imageFilePath);
    capture->capture();
}


void MainWindow::on_closeAct_triggered(bool checked)   //关闭摄像
{
    Camera->stop();
    captureSession->setVideoOutput(nullptr);
    outW->close();
    outW=new QVideoWidget(ui->groupBox_2);
    ui->groupBox_2->layout()->addWidget(outW);
    captureSession->setVideoOutput(outW);
}


void MainWindow::on_exitAct_triggered()
{
    this->close();
}


void MainWindow::on_chosesPathBtn_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,               // 父窗口
        tr("选择视频文件保存路径"),   // 对话框标题
        QString(),          // 默认路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks // 只显示文件夹
        );

    if (!folderPath.isEmpty()) {
        imageOutPutDir=folderPath;
        ui->defalutPathLabel->setText(QString("保存目录:%1").arg(imageOutPutDir));
    }
}

