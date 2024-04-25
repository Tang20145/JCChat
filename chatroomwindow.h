#ifndef CHATROOMWINDOW_H
#define CHATROOMWINDOW_H

#include <QWidget>
#include <QTcpSocket>


#include <QJsonObject>
#include <QJsonDocument>

//#include <cstring>

namespace Ui {
class ChatRoomWindow;
}

class ChatRoomWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoomWindow(QWidget *parent = nullptr);
    explicit ChatRoomWindow(QWidget *parent = nullptr,QTcpSocket* sock_ref = nullptr,QString user_name_ref = "");
    ~ChatRoomWindow();

private slots:
    void on_sendPushButton_clicked();
    void recv_handle();
private:
    void add_msg(QJsonObject json_msg);
private:
    Ui::ChatRoomWindow *ui;
    QTcpSocket* sock;
    QString user_name;
    QByteArray buffer;
};

#endif // CHATROOMWINDOW_H
