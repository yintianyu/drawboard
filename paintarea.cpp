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
    originPoint.setX(0);
    originPoint.setY(0);
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.scale(scale, scale);
    if(isDrawing == true)
    {
        paintAsAngle(painter, tempImage);
    }
    else
    {
        paintAsAngle(painter, image);
    }

}

void PaintArea::paintAsAngle(QPainter &painter, QImage &theImage)
{
    if(angle == 90)
    {
        painter.translate(originPoint);
        painter.shear(stretch, stretch);
        painter.rotate(angle);
        painter.drawImage(0, 0, theImage);
    }
    else if(angle == 180)
    {
        painter.translate(originPoint);
        painter.shear(stretch, stretch);
        painter.rotate(angle);
        painter.drawImage(0, 0, theImage);

    }
    else if(angle == 270)
    {
        painter.translate(originPoint);
        painter.shear(stretch, stretch);
        painter.rotate(angle);
        painter.drawImage(0, 0, theImage);
    }
    else if(angle == 0)
    {
        painter.shear(stretch, stretch);
        painter.drawImage(0, 0, theImage);
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

    qreal x, y, w, h, e_x, e_y;
    if(angle == 90)
    {
        x = lastPoint.y() / scale;
        y = (image.height() * scale - lastPoint.x()) / scale;
        e_x = endPoint.y() / scale;
        e_y = (image.height() * scale - endPoint.x()) / scale;
        w = e_x - x;
        h = e_y - y;
    }
    else if (angle == 180)
    {
        x = (originPoint.x() * scale - lastPoint.x()) / scale;
        y = (originPoint.y() * scale - lastPoint.y()) / scale;
        e_x = (originPoint.x() * scale - endPoint.x()) / scale;
        e_y = (originPoint.y() * scale - endPoint.y()) / scale;
        w = e_x - x;
        h = e_y - y;
    }
    else if(angle == 270)
    {
        x = (originPoint.y() * scale - lastPoint.y()) / scale;
        y = (lastPoint.x()) / scale;
        e_x = (originPoint.y() * scale - endPoint.y()) / scale;
        e_y = (endPoint.x()) / scale;
        w = e_x - x;
        h = e_y - y;
    }
    else
    {
        x = lastPoint.x() / scale;
        y = lastPoint.y() / scale;
        e_x = endPoint.x() / scale;
        e_y = endPoint.y() / scale;
        w = e_x - x;
        h = e_y - y;
    }

    switch(currentShape)
    {
    case None:
        pp.drawLine(x, y, e_x, e_y);
        lastPoint = endPoint;
        break;
    case Line:
        pp.drawLine(x, y, e_x, e_y);
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
    for(int i = 0;i < angle;i += 90)
    {
        copySize.transpose();
    }
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
    angle %= 360;
    switch(angle)
    {
    case 0:
        originPoint.setX(0);
        originPoint.setY(0);
        break;
    case 90:
        originPoint.setX(image.height());
        originPoint.setY(0);
        break;
    case 180:
        originPoint.setX(image.width());
        originPoint.setY(image.height());
        break;
    case 270:
        originPoint.setX(0);
        originPoint.setY(image.width());
        break;
    default:
        break;
    }

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


