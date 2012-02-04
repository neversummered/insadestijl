#include "renderwidget.h"
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QMouseEvent>

RenderWidget::RenderWidget(QWidget* parent) : QWidget(parent), Filter(), imageData(0), imageWidth(640), imageHeight(380) {
    setAttribute(Qt::WA_OpaquePaintEvent, true); // don't clear the area before the paintEvent
    setAttribute(Qt::WA_PaintOnScreen, true); // disable double buffering
    setFixedSize(imageWidth, imageHeight);
    time = frames = 0;
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event){
    int lPositionX = event->x();
    int lPositonY = event->y();
    QPoint point;
    point.setX(event->x());
    point.setY(event->y());
    qDebug() << "click on screen X:" << lPositionX << " Y:" << lPositonY;
    emit(clickOnArea(point));
}

void RenderWidget::onFrameSizeChanged(int width, int height) {
    setFixedSize(width, height);
} 

void RenderWidget::updatePixmap(const IplImage* frame) {
    QTime t;
    t.start();
    bool start = false;
    // check if the frame dimensions have changed
    if(frame->width != imageWidth || frame->height != imageHeight) {
        if(imageData) {
            delete[] imageData;
        }
        start = true;
        imageWidth = frame->width;
        imageHeight = frame->height;
        emit(frameSizeChanged(imageWidth, imageHeight));
        imageData = new unsigned char[4*imageWidth*imageHeight];
        for(int i = 0; i < imageWidth*imageHeight; i++) {
            imageData[i*4+3] = 0xFF;
        }
    }

    int pixels = imageWidth * imageHeight;
    uchar* src = (uchar*)(frame->imageData);
    uchar* srcEnd = src + (3*pixels);
    uchar* dest = imageData;

    do {
        memcpy(dest, src, 3);
        dest += 4;
        src += 3;
    } while(src < srcEnd);

    if(!start) {
        ++frames;
        time += t.elapsed();
    }
}

void RenderWidget::processPoint(HeadState* state) {
    // copy the image to the local pixmap and update the display
    updatePixmap(state->frame);
    update();

    notifyListener(state);
}

void RenderWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    if(imageData) {
        QImage tImg(imageData, imageWidth, imageHeight, QImage::Format_RGB32);
        painter.drawImage(QPoint(0,0), tImg);
    }
    else {
        painter.setBrush(Qt::black);
        painter.drawRect(rect());
    }
}
