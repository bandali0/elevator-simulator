#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "elevator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// register the slots (the functions that will be called by Qt framework when a specific and desired event happens)
private slots:
    void floorNumProcessor();
    void elevatorProcess();
    void outsideFloorProcessor();
    void on_btnGo_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
