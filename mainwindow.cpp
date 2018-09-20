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

    // Menu
    isSaved = false;
    currentFile = tr("untitled.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_N_triggered()
{
    doNew();
}

// Menu

void MainWindow::doOpen()
{
    if(whetherSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty())
        {
            area->openImage(fileName);
            scrollArea->widget()->resize(area->getImageSize()); // Get Image size and resize scrollArea
            isSaved = true;
            currentFile = fileName;
        }
    }
}

void MainWindow::doNew()
{
    if(whetherSave())
    {
        DoNewDialog dialog;
        if(dialog.exec() == QDialog::Accepted)
        {
            int width = dialog.getWidth();
            int height = dialog.getHeight();
            area->setImageSize(width, height);
            scrollArea->widget()->resize(width, height);
            area->setImageColor(dialog.getBackColor());
            isSaved = false;
        }
    }
}

bool MainWindow::whetherSave()
{
    if(area->isModified())
    {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this, tr("Save File"), tr("Image has been changed, save?"),\
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(box == QMessageBox::Yes)
        {
            return doFileSave();
        }
        else if(box == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::doFileSave()
{
    if(isSaved)
    {
        return saveFile(currentFile);
    }
    else
    {
        return doFileSaveAs();
    }
}

bool MainWindow::saveFile(QString fileName)
{
    if(area->saveImage(fileName, "png"))
    {
        isSaved = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::doFileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), currentFile);
    if(fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return saveFile(fileName);
    }
}

void MainWindow::on_actionOpen_O_triggered()
{
    doOpen();
}

void MainWindow::on_actionSave_S_triggered()
{
    doFileSave();
}

void MainWindow::on_actionSave_as_triggered()
{
    doFileSaveAs();
}

void MainWindow::on_actionExit_triggered()
{
    if(whetherSave())
    {
        qApp->quit();
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(whetherSave())
    {
        qApp->quit();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionPrint_P_triggered()
{
    area->doPrint();
}
