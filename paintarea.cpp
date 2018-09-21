#include "paintarea.h"
#include <iostream>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(400, 300, QImage::Format_RGB32);
    backColor = qRgb(255, 255, 255);
    image.fill(backColor);
    modified = false;

    //Edit Menu
    scale = 1;
    angle = 0;
    stretch = 0;

    // Tool bar
    penColor = Qt::black;
    brushColor = Qt::black;
    penWidth = 1;
    penStyle = Qt::SolidLine;
    currentShape = None;

    // Solve bug
    isDrawing = false;
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(scale, scale);
    if(isDrawing == true)
    {
        painter.drawImage(0, 0, tempImage);
    }
    else
    {
        if(angle)
        {
            QImage copyImage = image;
            QPainter pp(&copyImage);
            QPointF center(copyImage.width() / 2.0, copyImage.height() / 2.0);
            pp.translate(center);
            pp.rotate(angle);
            pp.translate(-center);
            pp.drawImage(0, 0, image);
            image = copyImage;
            angle = 0;
        }
        if(stretch != 0)
        {
            QImage copyImage = image;
            QPainter pp(&copyImage);
            pp.shear(stretch, stretch);
            pp.drawImage(0, 0, image);
            image = copyImage;
            stretch = 0;
        }
        painter.drawImage(0, 0, image);
    }
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        isDrawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        endPoint = event->pos();
        if(currentShape ==None)
        {
            isDrawing = false;
            paint(image);
        }
        else
        {
            tempImage = image;
            paint(tempImage);
        }

    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        endPoint = event->pos();
        isDrawing = false;
        paint(image);
    }
}

void PaintArea::paint(QImage &theImage)
{
    QPainter pp(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    pp.setPen(pen);
    pp.setBrush(brush);

    int x, y, w, h;
    x = lastPoint.x() / scale;
    y = lastPoint.y() / scale;
    w = endPoint.x() / scale - x;
    h = endPoint.y() / scale - y;

    switch(currentShape)
    {
    case None:
        pp.drawLine(lastPoint / scale, endPoint / scale);
        lastPoint = endPoint;
        break;
    case Line:
        pp.drawLine(lastPoint / scale, endPoint / scale);
        break;
    case Rectangle:
        pp.drawRect(x, y, w, h);
        break;
    case Ellipse:
        pp.drawEllipse(x, y, w, h);
        break;
    }
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
    QSize copySize = image.size();
    return copySize * scale;
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

// Edit Menu
void PaintArea::zoomIn()
{
    scale *= 1.2;
    update();
}

void PaintArea::zoomOut()
{
    scale /= 1.2;
    update();
}

void PaintArea::resume()
{
    scale = 1;
    update();
}

void PaintArea::doRotate()
{
    angle += 90;
    update();
}

void PaintArea::doStretch()
{
    stretch = 0.2;
    update();
}

void PaintArea::doClear()
{
    image.fill(backColor);
    update();
}

// Tool bar
void PaintArea::setPenStyle(Qt::PenStyle style)
{
    penStyle = style;
}

void PaintArea::setPenWidth(int width)
{
    penWidth = width;
}

void PaintArea::setPenColor(QColor color)
{
    penColor = color;
}

void PaintArea::setBrushColor(QColor color)
{
    brushColor = color;
}

void PaintArea::setShape(ShapeType shape)
{
    currentShape = shape;
}


