#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

void ChatWindow::init(QTcpSocket* sock_ref,QString username_ref)
{
    sock = sock_ref;
    username = username_ref;
    this->setWindowTitle(QString(username));
    ui->msgTextBrowser->append("已成功连接上服务器……");

    connect(sock,&QTcpSocket::readyRead,this,[=]{
        QByteArray readMsg =sock->readAll();
        ui->msgTextBrowser->append(QString(readMsg));
    });
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_sendPushButton_clicked()
{
    int msgLen;
    QString msgStr = ui->msgTextEdit->toPlainText();
    QByteArray msg = msgStr.toUtf8();
    QByteArray msgLenByte;
    msg.insert(0,username.toUtf8()+"：");

    //添加消息长度前缀
    msgLen = msg.size();
    msgLenByte.append((const char*)&msgLen,sizeof(msgLen));

    //sock->write(msgLenByte);
    sock->write(msg);
    sock->flush();

    ui->msgTextEdit->clear();

}

//将窗口关闭，也自动将socket连接断开
void ChatWindow::closeEvent(QCloseEvent* e)
{
    if(sock->state() == QAbstractSocket::ConnectedState)
    {
        sock->disconnectFromHost();
    }

    e->accept();
}
