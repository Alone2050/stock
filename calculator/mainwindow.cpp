#include "mainwindow.h"
#include "ui_mainwindow.h"

//Global var
double lastValue = 0.0;
//State
bool divTrgiier = false;
bool multTrgiier = false;
bool plusTrgiier = false;
bool minusTrgiier = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display->setText(QString::number(lastValue));

    QPushButton *numButtons[10];
    for (int i = 0; i < 10; i++)
    {
        QString butName = "dig" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
    connect(ui->equal, SIGNAL(released()),this,SLOT(EqualButtonPressed()));
    connect(ui->butPlus, SIGNAL(released()),this,SLOT(MathButtonPressed()));
    connect(ui->butDiv, SIGNAL(released()),this,SLOT(MathButtonPressed()));
    connect(ui->butMinus, SIGNAL(released()),this,SLOT(MathButtonPressed()));
    connect(ui->butMulti, SIGNAL(released()),this,SLOT(MathButtonPressed()));
    connect(ui->butCE, SIGNAL(released()),this,SLOT(ClearCalculation()));
    connect(ui->butClear, SIGNAL(released()),this,SLOT(ClearLine()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::NumPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->display->text();
    if(displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0)
    {
        ui->display->setText(butVal);
    } else {
        QString newVal = displayVal + butVal;
        double dblNewValue =newVal.toDouble();
        ui->display->setText(QString::number(dblNewValue,'g', 16));
    }
}

void MainWindow::EqualButtonPressed()
{
    double resultValue = 0.0;
    QString displayValue = ui->display->text();
    double dblDisplayValue = displayValue.toDouble();
    if(multTrgiier || divTrgiier || plusTrgiier || minusTrgiier)
    {
        if(minusTrgiier)
        {
            resultValue = lastValue - dblDisplayValue;
        } else if(plusTrgiier)
        {
            resultValue = lastValue + dblDisplayValue;
        } else if(divTrgiier)
        {
            resultValue = lastValue / dblDisplayValue;
        } else if(multTrgiier)
        {
            resultValue = lastValue * dblDisplayValue;
        }
        ui->display->setText(QString::number(resultValue,'g',16));
    }
}
void MainWindow::ChangeNumberSign(){}

void MainWindow::ClearCalculation()
{
    ui->display->clear();
    ui->display->setText("0.0");
    divTrgiier = false;
    multTrgiier = false;
    plusTrgiier = false;
    minusTrgiier = false;
}

void MainWindow::ClearLine()
{
    ui->display->clear();
}

void MainWindow::MathButtonPressed()
{
    divTrgiier = false;
    multTrgiier = false;
    plusTrgiier = false;
    minusTrgiier = false;
    QString displayValue = ui->display->text();
    lastValue = displayValue.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString buttonValue = button->text();
    if(QString::compare(buttonValue,"/",Qt::CaseInsensitive) == 0)
    {
        divTrgiier = true;
    } else if(QString::compare(buttonValue,"*",Qt::CaseInsensitive) == 0)
    {
        multTrgiier = true;
    } else if(QString::compare(buttonValue,"+",Qt::CaseInsensitive) == 0)
    {
        plusTrgiier = true;
    } else if(QString::compare(buttonValue,"-",Qt::CaseInsensitive) == 0)
    {
        minusTrgiier = true;
    }
    ui->display->clear();
}
