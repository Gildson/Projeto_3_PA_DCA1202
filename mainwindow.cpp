#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(ui->pushButtonStart,
          SIGNAL(clicked(bool)),
          this,
          SLOT(start()));

  connect(ui->pushButtonStop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(stop()));

  connect(ui->pushButtonConnect,
           SIGNAL(clicked(bool)),
           this,
           SLOT(tcpConnect()));

  connect(ui->pushButtonDisconnect,
           SIGNAL(clicked(bool)),
           this,
           SLOT(tcpDisconnect()));

  connect(ui->lcdNumberMax,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaMax()));

  connect(ui->lcdNumberMin,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaMin()));

  connect(ui->horizontalSliderTiming,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(mudaTempo(int)));
}

void MainWindow::tcpConnect(){
  socket->connectToHost(ui->lineEditIP->selectedText(), 1234);
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
    if(ui->lcdNumberMax->value() < ui->lcdNumberMin->value())
        ui->lcdNumberMax->setDigitCount(ui->lcdNumberMax->value()+1);
}

void MainWindow::ChangeMin()
{
    if(ui->lcdNumberMin->value() > ui->lcdNumberMax->value())
        ui->lcdNumberMin->setDigitCount(ui->lcdNumberMin->value()-1);
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
    tempo = _tempo;
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

  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    str = "set "+ QString::number(msecdate) + " " +
    QString::number(ui->horizontalSliderMin->value() +
    qrand()%(ui->horizontalSliderMax->value() - ui->horizontalSliderMin->value() +1))+"\r\n";
    socket->write(str.toStdString().c_str()) + " bytes written";


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
