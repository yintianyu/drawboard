#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QPrintDialog>
#include <QPrinter>
#include <QStack>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);
    void setImageSize(int width, int height);
    void setImageColor(QColor color);
    QColor getBackColor()
    {
        return QColor(backColor);
    }

    bool isModified() const // Inner Function
    {
        return modified;
    }
    bool saveImage(const QString &fileName, const char *fileFormat); // Save Image
    bool openImage(const QString &fileName); // Open Pictures
    QSize getImageSize();
    //Print
    void doPrint();

    // Edit Menu
    void zoomIn();
    void zoomOut();
    void resume();
    void doRotate();
    void doStretch();
    void doClear();

    // Advanced Tools
    void doGrey();
//    void doLight();

    // Tool bar
    void setPenStyle(Qt::PenStyle style);
    void setPenWidth(int width);
    void setPenColor(QColor color);
    void setBrushColor(QColor color);

    void redo();

    enum ShapeType
    {
        None,
        Line,
        Rectangle,
        Ellipse,
        Blur,
        Sharpen
    };
    void setShape(ShapeType shape);

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
    void paintAsAngle(QPainter &painter, QImage &theImage);

    // Advanced Tools
    QImage* greyScale(QImage *ogirin);
//    QImage* lightAjustment(QImage *origin, int position);
//    int lastLightDelta; // Record light delta last time
    QImage* blur(QImage *origin, QPoint point);
    QImage* sharpen(QImage *origin, QPoint point);

    // Edit Menu
    qreal scale;  // zoom scale
    int angle;  // rotate angle
    qreal stretch; // stretch value

    // Tool bar
    QColor penColor;
    QColor brushColor;
    int penWidth;
    Qt::PenStyle penStyle;
    ShapeType currentShape;

    // Sovle bug
    QImage tempImage;
    bool isDrawing;

    QPoint originPoint;

    QList<int> expandedImage;

    //Inline Functions
    template<typename T>
    T smallerOne(T a, T b)
    {
        if(a > b)
            return b;
        else
            return a;
    }

    template<typename T>
    T largerOne(T a, T b)
    {
        if(a > b)
            return a;
        else
            return b;
    }
    QStack<QImage> images;
};

#endif // PAINTAREA_H
