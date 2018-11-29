#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

private:
    int tempo;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void timerEvent(QTimerEvent *event);
  int timerId;
  void putData();
public slots:
  void tcpConnect();
  void tcpDisconnect();
  void ChangeMax();
  void ChangeMin();
  void ChangeTime(int _tempo);
  void start();
  void stop();

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;

};

#endif // MAINWINDOW_H
