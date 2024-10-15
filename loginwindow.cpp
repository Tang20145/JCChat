#include "loginwindow.h"
#include "ui_loginwindow.h"



LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    //界面整洁
    this->setWindowTitle("Login");
    this->setFixedHeight(210);
    ui->addressLabel->setAlignment(Qt::AlignRight);
    ui->portLabel->setAlignment(Qt::AlignRight);
    ui->usernameLabel->setAlignment(Qt::AlignRight);

    sock = nullptr;
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_connectPushButton_clicked()
{
    //发起连接
    sock = new QTcpSocket(this);

    if(sock->state() == QAbstractSocket::ConnectedState)
        sock->abort();

    QString ip = ui->addressLineEdit->text();
    unsigned short port = ui->portLineEdit->text().toUShort();
    sock->connectToHost(QHostAddress(ip),port);

    //连接按钮禁用
    ui->connectPushButton->setDisabled(true);

    connect(sock,&QTcpSocket::connected,this,[=]{
        this->close();
        chatwindow = new ChatWindow;
        chatwindow->show();
        chatwindow->init(sock,ui->usernameLineEdit->text());
    });

    connect(sock,&QTcpSocket::disconnected,this,[=]{
        this->show();
        sock->close();
        sock->deleteLater();
        sock = nullptr;

        chatwindow->close();

        QMessageBox msgBox;
        msgBox.setText("连接已断开");
        msgBox.exec();

        //恢复按钮连接
        ui->connectPushButton->setEnabled(true);
    });
}

