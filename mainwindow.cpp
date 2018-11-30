#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    connect(ui->pushButtonDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect()));

    connect(ui->horizontalSliderMax,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(ChangeMax()));

    connect(ui->horizontalSliderMin,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(ChangeMin()));

    connect(ui->horizontalSliderTiming,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(ChangeTime(int)));

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(start()));

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stop()));

}

void MainWindow::tcpConnect(){
    socket->connectToHost(ui->lineEditIP->displayText(), 1234);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
        statusBar()->showMessage("Connected");
    }
    else{
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();
    statusBar()->showMessage("Disconnected");
}

void MainWindow::ChangeMax()
{
    if(ui->lcdNumberMax->digitCount() < ui->lcdNumberMin->digitCount())
        ui->lcdNumberMax->setDigitCount(ui->lcdNumberMax->digitCount()+1);
}

void MainWindow::ChangeMin()
{
    if(ui->lcdNumberMin->digitCount() > ui->lcdNumberMax->digitCount())
        ui->lcdNumberMin->setDigitCount(ui->lcdNumberMin->digitCount()-1);
}

void MainWindow::start()
{
    tempo= startTimer(ui->horizontalSliderTiming->value()*10);
}

void MainWindow::stop()
{
    killTimer(tempo);
}

void MainWindow::ChangeTime(int _tempo)
{
    tempo = _tempo-1;
    killTimer(timerId);
    timerId = startTimer(1000*tempo);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    putData();
}

void MainWindow::putData(){
    QDateTime datetime;
    QString str;
    qint64 msecdate;
    QStringList list;

    int min = ui->horizontalSliderMin->value();
    int max = ui->horizontalSliderMax->value();

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " + QString::number(min + (qrand() % (max - min + 1)))+"\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000))
        {
            qDebug() << "wrote";
        }

        list.append(str);
        if(socket->waitForBytesWritten(3000)){
        }
        ui->listWidgetSet->addItems(list);
    }
}

MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
