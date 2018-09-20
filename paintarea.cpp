#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(400, 300, QImage::Format_RGB32);
    backColor = qRgb(255, 255, 255);
    image.fill(backColor);
    modified = false;
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
    modified = true;
}

void PaintArea::setImageSize(int width, int height)
{
    QImage newImage(width, height, QImage::Format_RGB32);
    image = newImage;
    update();
}

void PaintArea::setImageColor(QColor color)
{
    backColor = color.rgb(); // convert the color to rgb type
    image.fill(backColor);
    update();
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    if(visibleImage.save(fileName, fileFormat))
    {
        modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName))
    {
        return false;
    }
    QSize newSize = loadedImage.size();
    setImageSize(newSize.width(), newSize.height());
    image = loadedImage;
    modified = false;
    update();
    return true;
}

QSize PaintArea::getImageSize()
{
    return image.size();
}

void PaintArea::doPrint()
{
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    if(printDialog->exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}
