#include <math.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    newAct = new QAction(tr("&Новый"), this);
    fileMenu = this->menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAct);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int GetNod(int ch,int zn) { //Найти НОД
 return (ch ? GetNod(zn%ch,ch) : zn);
 }

QString ClipFraction (int ch,int zn) { //Сократить, если есть НОД>1
 int nod = GetNod(ch,zn);
 if (nod>1) { ch/=nod; zn/=nod; }
 QString result;
 if (ch==0) result.setNum(0);
 else if (ch==zn) result.setNum(1);
 else if (zn==1) result.setNum(ch);
 else result = QString("%1/%2").arg(ch).arg(zn);
 return result;
}

void MainWindow::newFile() {
    //1. Подготовить данные x,y:
    const int size = 360;
    QVector <double> x(size),y(size);
    for (int i=0; i<size; i++) {
     x[i] = i;
     y[i] = sin(qDegreesToRadians(x[i]));
    }
    //2. Создать график и добавить данные:
    ui->centralWidget->addGraph();
    ui->centralWidget->graph(0)->setData(x,y);
    //3. Настроить оси и подписи:
    ui->centralWidget->xAxis->setLabel("Ось x");
    ui->centralWidget->xAxis->setRange(0,size); //1
/*
    QVector<double> Ticks; // вектор с шагом в 30 градусов
    for (int i=0; i<=size; i+=30) Ticks << i;
    ui->centralWidget->xAxis->setAutoTicks(false); // выключаем автоматические отсчеты
    ui->centralWidget->xAxis->setTickVector(Ticks);  // задаем созданный нами вектор
*/
    //Оси в долях числа Пи
    QVector <QString> Labels;
    for (int i=0; i<=size; i+=size/9) Labels << ClipFraction(i,size/2) + QChar(960);
    ui->centralWidget->xAxis->setAutoTickLabels(false);
    ui->centralWidget->xAxis->setTickVectorLabels(Labels);

    ui->centralWidget->yAxis->setLabel("f(x)");
    ui->centralWidget->yAxis->setRange(-1,1);
    //4. Показать всё это:
    ui->centralWidget->replot();

}
