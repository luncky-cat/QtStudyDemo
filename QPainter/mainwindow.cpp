#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QPainter>
#include<QColor>
#include <Qpen>
#include <QPainterPath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::paintEvent(QPaintEvent *event)    //一般效果
// {
//     QBrush brush;
//     brush.setColor(Qt::red);
//     brush.setStyle(Qt::SolidPattern);  // 实心填充


//     // 设置画笔颜色为绿色，设置边框宽度为 10
//     QPen pen;
//     pen.setColor(Qt::green);   // 设置边框颜色为绿色
//     pen.setWidth(10);          // 设置边框宽度为 10
//     pen.setCapStyle(Qt::PenCapStyle::RoundCap);
//     pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
//     pen.setStyle(Qt::PenStyle::DotLine);


//     QPainter painter(this);
//     painter.setPen(pen);       // 设置画笔
//     painter.setBrush(brush);   // 设置填充颜色为红色

//     QRect rect(100, 100, 100, 100); // 矩形的位置和大小
//     painter.drawRect(rect);    // 绘制矩形，边框为绿色，填充为红色
// }

// void MainWindow::paintEvent(QPaintEvent *event)   //线性
// {
//     QPainter painter(this);  // 关联到当前窗口

//     // 创建线性渐变填充：从 (0,0) 到 (200, 0)
//     QLinearGradient gradient(0, 0, 200, 0); // 渐变从 (0,0) 到 (200,0)
//     gradient.setColorAt(0, Qt::red);  // 起始颜色为红色
//     gradient.setColorAt(1, Qt::yellow);  // 结束颜色为黄色

//     // 使用渐变填充
//     QBrush brush(gradient);

//     // 设置画笔为绿色边框，宽度为 5
//     QPen pen;
//     pen.setColor(Qt::green);
//     pen.setWidth(3);
//     painter.setPen(pen);
//     painter.setBrush(brush);  // 使用渐变填充

//     QRect rect(50, 50, 300, 100);  // 矩形的位置和大小
//     painter.drawRect(rect);  // 绘制矩形，使用渐变填充
// }

// void MainWindow::paintEvent(QPaintEvent *event){
//     QPainter painter(this);  // 关联到当前窗口

//     // 创建径向渐变：渐变从圆心向外扩展
//     QRadialGradient gradient(this->width()/2, this->height()/2, qMax(this->width()/8, this->height()/8), this->width()/2, this->height()/2);
//     gradient.setColorAt(0, Qt::red);  // 起始颜色为红色
//     gradient.setColorAt(0.5,Qt::green);
//     gradient.setColorAt(1, Qt::yellow);  // 结束颜色为黄色
//     gradient.setSpread(QGradient::ReflectSpread);  // 设置渐变的扩展方式

//     // 使用渐变填充
//     QBrush brush(gradient);

//     // 设置画笔为绿色边框，宽度为 5
//     QPen pen;
//     pen.setColor(Qt::green);  // 设置边框颜色为绿色
//     painter.setPen(pen);
//     painter.setBrush(brush);  // 使用渐变填充

//     int radius = qMin(this->width(), this->height()) / 4;  // 使用窗口大小的一部分来设置圆的半径
//     QRect rect(this->width()/2 - radius, this->height()/2 - radius, 2*radius, 2*radius);  // 计算圆的矩形区域

//     painter.drawEllipse(rect);  // 绘制圆形，使用渐变填充
// }


// void MainWindow::paintEvent(QPaintEvent *event)     //五角星旋转，平移
// {

//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);

//     int spacing = width() / 4;
//     qreal radius = 80;

//     // --------- 工具函数：创建五角星路径和点 ---------
//     auto createStarPoints = [](QPointF center, qreal r) -> QVector<QPointF> {
//         QVector<QPointF> points;
//         for (int i = 0; i < 5; ++i) {
//             qreal angleDeg = 72 * i - 90;
//             qreal angleRad = qDegreesToRadians(angleDeg);
//             qreal x = center.x() + r * cos(angleRad);
//             qreal y = center.y() + r * sin(angleRad);
//             points.append(QPointF(x, y));
//         }
//         return points;
//     };

//     auto createStarPath = [](const QVector<QPointF>& pts) -> QPainterPath {
//         QPainterPath path;
//         path.moveTo(pts[0]);
//         path.lineTo(pts[2]);
//         path.lineTo(pts[4]);
//         path.lineTo(pts[1]);
//         path.lineTo(pts[3]);
//         path.closeSubpath();
//         return path;
//     };

//     // --------- 星1 ---------
//     QPointF center1(spacing, height() / 2);
//     QVector<QPointF> pts1 = createStarPoints(center1, radius);
//     QPainterPath path1 = createStarPath(pts1);
//     painter.setPen(Qt::blue);
//     painter.drawPath(path1);
//     for (int i = 0; i < 5; ++i)
//         painter.drawText(pts1[i] + QPointF(5, -5), QString::number(i + 1));
//     painter.drawText(center1 + QPointF(-20, -radius - 20), "星 1");

//     // --------- 星2：旋转 90°, 缩小 0.8 ---------
//     QPointF center2(spacing * 2, height() / 2);
//     QTransform transform2;
//     transform2.translate(center2.x(), center2.y());
//     transform2.rotate(90);
//     transform2.scale(0.8, 0.8);
//     transform2.translate(-center2.x(), -center2.y());

//     QVector<QPointF> pts2;
//     for (const auto& pt : createStarPoints(center2, radius))
//         pts2.append(transform2.map(pt));

//     QPainterPath path2 = createStarPath(pts2);
//     painter.setPen(Qt::red);
//     painter.drawPath(path2);
//     for (int i = 0; i < 5; ++i)
//         painter.drawText(pts2[i] + QPointF(5, -5), QString::number(i + 1));
//     painter.drawText(center2 + QPointF(-20, -radius - 20), "星 2");

//     // --------- 星3：在星2基础上逆时针旋转 145° ---------
//     QPointF center3(spacing * 3, height() / 2);
//     QTransform transform3;
//     transform3.translate(center3.x(), center3.y());
//     transform3.rotate(90 - 145); // 等价于 -55°
//     transform3.scale(0.8, 0.8);
//     transform3.translate(-center3.x(), -center3.y());

//     QVector<QPointF> pts3;
//     for (const auto& pt : createStarPoints(center3, radius))
//         pts3.append(transform3.map(pt));

//     QPainterPath path3 = createStarPath(pts3);
//     painter.setPen(Qt::darkGreen);
//     painter.drawPath(path3);
//     for (int i = 0; i < 5; ++i)
//         painter.drawText(pts3[i] + QPointF(5, -5), QString::number(i + 1));
//     painter.drawText(center3 + QPointF(-20, -radius - 20), "星 3");
// }

void MainWindow::paintEvent(QPaintEvent *event)
{


}

