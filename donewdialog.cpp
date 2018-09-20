#include "donewdialog.h"
#include "ui_donewdialog.h"

DoNewDialog::DoNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoNewDialog)
{
    ui->setupUi(this);
    backColor = Qt::white;
}

int DoNewDialog::getWidth()
{
    return ui->widthSpinBox->text().toInt();
}

int DoNewDialog::getHeight()
{
    return ui->heightSpinBox->text().toInt();
}

QColor DoNewDialog::getBackColor()
{
    return backColor;
}

void DoNewDialog::on_toolButton_clicked()
{
    QColor newColor = QColorDialog::getColor();
    if(newColor.isValid())
    {
        backColor = newColor;

        QPalette palette = ui->textBrowser->palette(); // Show this color
        palette.setColor(QPalette::Base, backColor);
        ui->textBrowser->setPalette(palette);
        update();
    }
}

DoNewDialog::~DoNewDialog()
{
    delete ui;
}
