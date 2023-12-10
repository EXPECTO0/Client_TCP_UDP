#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TCP Part
    TCP_Socket = new QTcpSocket();

    connect(TCP_Socket, &QTcpSocket::readyRead, this, &MainWindow::readTcpData);
    connect(TCP_Socket, &QTcpSocket::disconnected, this, &MainWindow::discardTcpSocket);

    TCP_Socket->connectToHost(QHostAddress(IPAddress), TcpPort);



    connect(ui->pushButton_SendTcpMessage, &QPushButton::clicked, this, &MainWindow::sendTcpMessage);


    //UDP Part
    UDP_Socket = new QUdpSocket();

    if(UDP_Socket->bind(QHostAddress::Any, 44222)){
         connect(UDP_Socket, &QUdpSocket::readyRead, this, &MainWindow::readUdpData);
    }else{
        QMessageBox::information(this, "UDP Client Error", UDP_Socket->errorString());
    }

    connect(ui->pushButton_SendUdpMessage, &QPushButton::clicked, this, &MainWindow::sendUdpMessage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readTcpData(){

    QByteArray DataBuffer;
    QDataStream socketstream(TCP_Socket);

    socketstream.startTransaction();
    socketstream >> DataBuffer;

    if(socketstream.commitTransaction() == false){
        return;
    }

    QString receivedMessage = QString::fromUtf8(DataBuffer);
    ui->textBrowser_TcpInbox->append("Server:  \n" + receivedMessage + "\n");

}

void MainWindow::discardTcpSocket(){

    TCP_Socket->deleteLater();

}


void MainWindow::sendTcpMessage(){

    if(TCP_Socket && TCP_Socket->isOpen()){

        QString message = ui->textEdit_TcpMessage->toPlainText();
        QByteArray messageData = message.toUtf8();
        QDataStream socketStream(TCP_Socket);
        socketStream << messageData;
    }

}


void MainWindow::readUdpData(){

    while(UDP_Socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(UDP_Socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;

        UDP_Socket->readDatagram(datagram.data(), datagram.size(),&senderAddress, &senderPort);



        QString receivedMessage = QString::fromUtf8(datagram);
        ui->textBrowser_UDPInbox->append("Server: \n" + receivedMessage);
    }

}

void MainWindow::sendUdpMessage(){

    if(UDP_Socket){
        QString message = ui->textEdit_UdpMessage->toPlainText();

        QByteArray messageData =  message.toUtf8();
        QHostAddress udpHostAddress(IPAddress);
        UDP_Socket->writeDatagram(messageData, udpHostAddress, ReceiverUdpPort);
    }

}

















































