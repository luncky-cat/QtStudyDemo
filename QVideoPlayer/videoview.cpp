#include "videoview.h"
#include <QMouseEvent>

videoview::videoview(QWidget *parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent);  // 确保控件不被遮挡
    setMouseTracking(true);  // 启用鼠标跟踪
}

void videoview::mouseDoubleClickEvent(QMouseEvent *event)
{
        QWidget::mouseDoubleClickEvent(event);
        if (this->isFullScreen()) {
            this->showNormal();  // 恢复到正常模式
        } else {
            this->showFullScreen();  // 进入全屏模式
        }
}

void videoview::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Escape||event->key() == Qt::Key_Enter) {
       this->showNormal();
    }
}



