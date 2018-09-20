#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(400, 300, QImage::Format_RGB32);
    backColor = qRgb(255, 255, 255);
    image.fill(backColor);
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();
        paint(image);
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        endPoint = event->pos();
        paint(image);
    }
}

void PaintArea::paint(QImage &theImage)
{
    QPainter pp(&theImage);
    pp.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
    update();
}

