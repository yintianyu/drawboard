#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(700, 500);
    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area);
    scrollArea->widget()->setMinimumSize(800, 600); //scrollArea initial size 800, 600
    setCentralWidget(scrollArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_N_triggered()
{
    DoNewDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        area->setImageSize(width, height);
        scrollArea->widget()->resize(width, height);
        area->setImageColor(dialog.getBackColor());
    }
}
