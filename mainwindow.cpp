#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1920, 1080);
    area = new PaintArea;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(area);
    scrollArea->widget()->setMinimumSize(800, 600); //scrollArea initial size 800, 600
    setCentralWidget(scrollArea);

    // Menu
    isSaved = false;
    currentFile = tr("untitled.png");

    // Tool Bar
    creatColorComboBox(ui->penColorComboBox);
    creatColorComboBox(ui->brushColorComboBox);

//    scrollArea->setWidgetResizable(true);
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

// Tool Bar

void MainWindow::creatColorComboBox(QComboBox *comboBox)
{
    QPixmap pixel(16, 16);
    QPainter painter(&pixel);

    painter.fillRect(0, 0, 16, 16, Qt::black);
    comboBox->addItem(QIcon(pixel), tr("Black"), QColor(Qt::black));
    painter.fillRect(0, 0, 16, 16, Qt::white);
    comboBox->addItem(QIcon(pixel), tr("White"), QColor(Qt::white));
    painter.fillRect(0, 0, 16, 16, Qt::red);
    comboBox->addItem(QIcon(pixel), tr("Red"), QColor(Qt::red));
    painter.fillRect(0, 0, 16, 16, Qt::green);
    comboBox->addItem(QIcon(pixel), tr("Green"), QColor(Qt::green));
    painter.fillRect(0, 0, 16, 16, Qt::blue);
    comboBox->addItem(QIcon(pixel), tr("Blue"), QColor(Qt::blue));
    painter.fillRect(0, 0, 16, 16, Qt::yellow);
    comboBox->addItem(QIcon(pixel), tr("Yellow"), QColor(Qt::yellow));
    painter.fillRect(0, 0, 16, 16, area->getBackColor());
    comboBox->addItem(QIcon(pixel), tr("Transparent"), QColor(Qt::transparent));
}

// Slot
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


void MainWindow::on_actionPrint_P_triggered()
{
    area->doPrint();
}

void MainWindow::on_actionZoom_In_triggered()
{
    area->zoomIn();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_actionZoom_Out_triggered()
{
    area->zoomOut();
    scrollArea->widget()->resize(area->getImageSize());
}

void MainWindow::on_actionResume_triggered()
{
    area->resume();
}

void MainWindow::on_actionRotate_triggered()
{
    area->doRotate();
    scrollArea->widget()->resize(area->getImageSize());
    update();
}

void MainWindow::on_actionStretch_triggered()
{
    area->doStretch();
}

void MainWindow::on_actionClear_triggered()
{
    area->doClear();
}

void MainWindow::on_actionDraw_Tool_Bar_triggered()
{
    ui->dockWidget->show();
}

void MainWindow::on_shapeComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("None"))
    {
        area->setShape(PaintArea::None);
    }
    else if(arg1 == tr("Line"))
    {
        area->setShape(PaintArea::Line);
    }
    else if(arg1 == tr("Rectangle"))
    {
        area->setShape(PaintArea::Rectangle);
    }
    else if(arg1 == tr("Ellipse"))
    {
        area->setShape(PaintArea::Ellipse);
    }
}

void MainWindow::on_penStyleComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("Solid Line"))
    {
        area->setPenStyle(Qt::SolidLine);
    }
    else if(arg1 == tr("Dot Line"))
    {
        area->setPenStyle(Qt::DotLine);
    }
}

void MainWindow::on_penWidthSpinBox_valueChanged(int arg1)
{
    area->setPenWidth(arg1);
}

void MainWindow::on_penColorComboBox_currentIndexChanged(int index)
{
    QColor color=ui->penColorComboBox->itemData(index, Qt::UserRole).value<QColor>();
    area->setPenColor(color);
}

void MainWindow::on_brushColorComboBox_currentIndexChanged(int index)
{
    QColor color = ui->brushColorComboBox->itemData(index, Qt::UserRole).value<QColor>();
    area->setBrushColor(color);
}
