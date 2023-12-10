#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QByteArray>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void discardTcpSocket();

    void sendTcpMessage();
    void readTcpData();

    void readUdpData();
    void sendUdpMessage();

private:
    Ui::MainWindow *ui;
    QTcpSocket *TCP_Socket;
    QUdpSocket *UDP_Socket;

    QString IPAddress = "192.168.10.246"; // Server IP Address for Tcp Connection

    quint16 TcpPort = 44000;
    quint16 UdpPort = 44222; // Server Udp port to send Udp Message
    quint16 ReceiverUdpPort = 44111; // Client Udp port to take Udp Message

};

#endif // MAINWINDOW_H
