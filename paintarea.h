#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QPrintDialog>
#include <QPrinter>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);
    void setImageSize(int width, int height);
    void setImageColor(QColor color);
    bool isModified() const // Inner Function
    {
        return modified;
    }
    bool saveImage(const QString &fileName, const char *fileFormat); // Save Image
    bool openImage(const QString &fileName); // Open Pictures
    QSize getImageSize();
    //Print
    void doPrint();

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QImage image;
    QRgb backColor;
    QPoint lastPoint, endPoint;
    bool modified;
    void paint(QImage &theImage);
};

#endif // PAINTAREA_H
