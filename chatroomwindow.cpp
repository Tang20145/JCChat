#include "chatroomwindow.h"
#include "ui_chatroomwindow.h"

ChatRoomWindow::ChatRoomWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoomWindow)
{
    ui->setupUi(this);
}

ChatRoomWindow::ChatRoomWindow(QWidget *parent,QTcpSocket* sock_ref,QString user_name_ref) :
    QWidget(parent),
    ui(new Ui::ChatRoomWindow),
    sock(sock_ref),
    user_name(user_name_ref)
{
    ui->setupUi(this);
    connect(sock,&QTcpSocket::readyRead,this,&ChatRoomWindow::recv_handle);

}



ChatRoomWindow::~ChatRoomWindow()
{
    delete ui;
}

void ChatRoomWindow::on_sendPushButton_clicked()
{
    QString msg_str = ui->msgTextEdit->toPlainText();
    QJsonObject json_msg;
    json_msg.insert("msg",msg_str);
    json_msg.insert("user_name",user_name);

    QByteArray send_data = QJsonDocument(json_msg).toJson();
    int data_len = send_data.length();

    sock->write((const char*)&data_len,sizeof(int));
    sock->write(send_data);

}

void ChatRoomWindow::recv_handle()
{
    //接收完整的信息
    buffer.append(sock->readAll());

    if(buffer.length() <= sizeof(int))//连消息长度都没接收到
        return;

    int len;
    memcpy(&len,buffer.constData(),sizeof(int));

    if(buffer.length() < sizeof(int) + len )//还没接收完全部消息
        return;

    QByteArray recv_data = buffer.mid(sizeof(int),len);
    buffer.remove(0,sizeof(int)+len);

    qDebug() << recv_data;

    //转换成json对象，显示在浏览框中
    QJsonObject json_msg = QJsonDocument::fromJson(recv_data).object();
    add_msg(json_msg);

    ui->msgTextEdit->clear();
}

void ChatRoomWindow::add_msg(QJsonObject json_msg)
{
    //QTextCursor cursor = ui->chatTextBrowser->textCursor();
    //cursor.movePosition(QTextCursor::End);
    QString msg_str = json_msg.value("msg").toString();
    QString user_name_recv = json_msg.value("user_name").toString();

    ui->chatTextBrowser->append("[ "+user_name_recv+" ]:");
    //ui->chatTextBrowser->append("");
    ui->chatTextBrowser->append(msg_str);
    //ui->chatTextBrowser->append("");

}
