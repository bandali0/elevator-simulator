/*************************************************************************************************************

  History: Amin Bandali - January 21, 2013
  Purpose: To simulate the functionality of an elevator in a building. Written using OOP technique.
  Input:    The number checkboxes inside the elvator (left part of window). Up/Down checkboxes in each floor.
  Output:   The current floor. Elevator's stop on requested floors

*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include "elevator.h"
#include <QTimer>
#include <QRegExp>
using namespace std;

// prototypes of some functions that are used in this file. Functions are declared in functions.cpp
QString toQString(std::string const &s);
std::string fromQString(QString const &s);
string intToString (int);

Elevator e1(1); // create an instance of the elevator class

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect signals from twelve numbers in the numeric pad to a slot
    // the slot function will be executed each time a signal is emitted
    QList<QCheckBox *> allNums = ui->groupBox->findChildren<QCheckBox *>();
    foreach(QCheckBox * cbNum, allNums)
        QObject::connect(cbNum,SIGNAL(toggled(bool)),this,SLOT(floorNumProcessor()));

    // Connects signals of disabling up and down floors
    QRegExp rxgB("gB");
    QRegExp rxu("u");
    QRegExp rxd("d");
    QList<QGroupBox *> allFloors = ui->centralWidget->findChildren<QGroupBox *>(rxgB);
    foreach(QGroupBox * gBFloor, allFloors)
    {
        QList<QCheckBox *> cBus = gBFloor->findChildren<QCheckBox *>(rxu);
        QList<QCheckBox *> cBds = gBFloor->findChildren<QCheckBox *>(rxd);
        foreach(QCheckBox * cBu, cBus)
            QObject::connect(cBu,SIGNAL(toggled(bool)),this,SLOT(outsideFloorProcessor()));
        foreach(QCheckBox * cBd, cBds)
            QObject::connect(cBd,SIGNAL(toggled(bool)),this,SLOT(outsideFloorProcessor()));
    }

    /*

    WHAT ARE SIGNALS AND SLOTS?

    Basically, signals and slots are used to instantly determine when a specific action has taken place.
    Qt will emit a signal when a specific event takes place. signals are connected to slots.
    A slot is almost like a normal function and it'll be called whenever the specified action takes place and signals are emitted.

    Signals and slots are alternatives to event handling in microsoft visual studio.

    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::floorNumProcessor()
{
    // create a pointer to the sender of the signal (the number that was clicked)
    QCheckBox * tempNum = dynamic_cast<QCheckBox *>(QObject::sender());

    if (tempNum->isChecked()) // if the number was clicked before
    {
        // Disables the number to not let the user cancel the operation by clicking again.
        // Similar to real elevators. once a floor button is pressed, the elevator WILL stop on that floor.
        tempNum->setEnabled(false);

        string num = fromQString(tempNum->objectName()); // converts the name of the object from QString format to C++ string
        num = num.substr(3,num.length()-3); // seperates the number part from the rest of the name and overwrites num

        int number = atoi(num.c_str()); // converts num (string) to an integer (number)

        e1.addFloor(number); // calls the addFloor function to add the number to the stop list
    }
}

// a function simliar to the one that processes clicks on the floor in the numbers pad (floorNumProcessor)
// but this one processes clicks on UP or DOWN buttons of each floor
void MainWindow::outsideFloorProcessor()
{
    // a pointer to the up (or down) checkbox that was clicked
    QCheckBox * tempFloor = dynamic_cast<QCheckBox *>(QObject::sender());

    if (tempFloor->isChecked()) // if the checkbox is checked
    {
        tempFloor->setEnabled(false); // disables it

        string num = fromQString(tempFloor->objectName()); // converts the name of the object from QString format to C++ string

        bool isUpOrDown; // a boolean variable that will be set true if up was checked or false if down was checked

        // determine whether the checkebox that was clicked is an up or a down (using the its name)
        if (num.substr(0,1) == "u")
            isUpOrDown = true; // true for up
        else if (num.substr(0,1) == "d")
            isUpOrDown = false; // false for down

        num = num.substr(1,num.length()-1); // seperates the number part from the rest of the name and overwrites num
        int number = atoi(num.c_str()); // converts num (string) to an integer (number)

        e1.addOutsideFloor(number, isUpOrDown); // calls the addOutsideFloor member function to process the request
    }
}

void MainWindow::elevatorProcess()
{
    bool process = e1.move(); // calls the move member function of elevator to move the elvator

    if (process) // process will be true if the elevator has to stop on the current floor
    {
        QTreeWidgetItem *logs = new QTreeWidgetItem(ui->treeSteps); // creates a pointer to the listbox of logs
        logs->setText(0,toQString("stop at " + intToString(e1.getCurFloor()))); // adds a line to logs

        // creates a pointer to the checkbox of the current floor (in the numbers pad)
        QCheckBox* cbnum = ui->groupBox->findChild<QCheckBox*>(toQString("npF"+intToString(e1.getCurFloor())));
        cbnum->setChecked(false); // unchecks it (removes the check mark)
        cbnum->setEnabled(true); // re-enables it


        // creates a pointer to the group of objects associated with the current floor
        QGroupBox * gBCurFloor = ui->centralWidget->findChild<QGroupBox *>(toQString("gB"+intToString(e1.getCurFloor())));

        // creates RegExps (used for searching in objects)
        QRegExp rxu("u"); // up
        QRegExp rxd("d"); // down

        // creates a pointer to all of objects of the groupbox of the current floor that their name starts with "u"
        QList<QCheckBox *> cBus = gBCurFloor->findChildren<QCheckBox *>(rxu);

        // creates a pointer to all of objects of the groupbox of the current floor that their name starts with "d"
        QList<QCheckBox *> cBds = gBCurFloor->findChildren<QCheckBox *>(rxd);

        // unchecks the UP checkbox of the current floor (removes the check mark) and re-enables it
        foreach(QCheckBox * cBu, cBus)
            {cBu->setChecked(false);cBu->setEnabled(true);}

        // unchecks the UP checkbox of the current floor (removes the check mark) and re-enables it
        foreach(QCheckBox * cBd, cBds)
            {cBd->setChecked(false);cBd->setEnabled(true);}

        e1.deleteFloor(e1.getCurFloor()); // removes the current floor from the stop list
    }

    ui->lblCurFloor->setText(toQString(intToString(e1.getCurFloor()))); // update the text of the label to the current floor

    // turns off the light of previous floor (change the color of image to grayscale)
    QGroupBox * gBPrevFloor = ui->centralWidget->findChild<QGroupBox *>(toQString("gB"+intToString(e1.getPrevFloor())));
    QPushButton * pBPrevFloor = gBPrevFloor->findChild<QPushButton *>(toQString("pB"+intToString(e1.getPrevFloor())));
    pBPrevFloor->setEnabled(false);

    // turns on the light of current floor (change the color of image from grayscale to original (which is green) )
    QGroupBox * gBCurFloor = ui->centralWidget->findChild<QGroupBox *>(toQString("gB"+intToString(e1.getCurFloor())));
    QPushButton * pBCurFloor = gBCurFloor->findChild<QPushButton *>(toQString("pB"+intToString(e1.getCurFloor())));
    pBCurFloor->setEnabled(true);
}

void MainWindow::on_btnGo_clicked()
{
    // creates a timer to call the function of processing the elevator
    QTimer *timer = new QTimer(this);

    // connects the timer to the elevatorProcess function.
    // Every time that desired interval passes, the function will be called
    connect(timer, SIGNAL(timeout()), this, SLOT(elevatorProcess()));

    // starts the timer with the interval of 1000 mili-seconds (or 1second)
    timer->start(1000);

    /*
        Basically what the timer above does is that it calls the elevatorProcess function every one second.
        The advantage of using timers is that they don't stop the flow of the program.
        They will be executed and processed parallel with the main program flow.

        If sleep function was to be used, the program would always be non-responding when elevators were being processed
    */

    // creates a pointer to the listbox of logs
    QTreeWidgetItem *logs = new QTreeWidgetItem(ui->treeSteps);

    // adds an item (a line) with a text of "start"
    logs->setText(0,toQString("start"));

    // starts the elevator
    e1.start();

    // disables the "Go" button
    ui->btnGo->setEnabled(false);
}
