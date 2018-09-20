#ifndef DONEWDIALOG_H
#define DONEWDIALOG_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class DoNewDialog;
}

class DoNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoNewDialog(QWidget *parent = 0);
    int getWidth();
    int getHeight();
    QColor getBackColor();
    ~DoNewDialog();

private slots:
    void on_toolButton_clicked();

private:
    Ui::DoNewDialog *ui;
    QColor backColor;
};

#endif // DONEWDIALOG_H
