#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QPainter>
#include "paintarea.h"
#include "donewdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Menu
    void doNew(); // New an image
    void doOpen(); // Open File
    bool doFileSave(); // Save File
    bool doFileSaveAs(); // Save as

    // Tool Bar
    void creatColorComboBox(QComboBox *comboBox);  // Build up color combination box

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_N_triggered();

    void on_actionOpen_O_triggered();

    void on_actionSave_S_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionPrint_P_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionResume_triggered();

    void on_actionRotate_triggered();

    void on_actionStretch_triggered();

    void on_actionClear_triggered();

    void on_actionDraw_Tool_Bar_triggered();

    void on_shapeComboBox_currentIndexChanged(const QString &arg1);

    void on_penStyleComboBox_currentIndexChanged(const QString &arg1);

    void on_penWidthSpinBox_valueChanged(int arg1);

    void on_penColorComboBox_currentIndexChanged(int index);

    void on_brushColorComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    PaintArea *area;
    QScrollArea *scrollArea;

    // Menu
    bool isSaved;
    QString currentFile;
    bool whetherSave();

    bool saveFile(QString fileName);
};

#endif // MAINWINDOW_H
