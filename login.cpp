#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    sock = new QTcpSocket;

    connect(sock,&QTcpSocket::connected,this,&Login::connect_handle);
    connect(sock,&QTcpSocket::disconnected,this,&Login::disconnect_msg_box);


}

Login::~Login()
{
    delete ui;
}

void Login::connect_handle()
{
    QMessageBox::information(NULL,"连接提示","连接成功");
    chatwindow = new ChatRoomWindow(NULL,sock,user_name);
    this->hide();
    chatwindow->show();
}

void Login::disconnect_msg_box()
{
    QMessageBox::warning(NULL,"连接提示","异常！连接已断开!");
}


void Login::on_connectPushButton_clicked()
{
    QString ip_str = ui->ipLineEdit->text();
    QString port_str = ui->portLineEdit->text();
    user_name = ui->usernameLineEdit->text();
    sock->connectToHost(QHostAddress(ip_str),port_str.toShort());


}
