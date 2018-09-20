#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);

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
    void paint(QImage &theImage);
};

#endif // PAINTAREA_H
