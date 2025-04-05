#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H
#include <QMouseEvent>
#include<QVideoWidget>


class videoview : public QVideoWidget
{
    Q_OBJECT
public:
    // 修改构造函数，接受 QWidget* 类型的父控件
    explicit videoview(QWidget *parent = nullptr);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
};

#endif // VIDEOVIEW_H
