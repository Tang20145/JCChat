#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include <QTcpSocket>
#include <QHostAddress>

#include <QMessageBox>

#include "chatroomwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_connectPushButton_clicked();
    void connect_handle();
    void disconnect_msg_box();

private:
    Ui::Login *ui;
    QTcpSocket* sock;
    ChatRoomWindow* chatwindow;
    QString user_name;
};
#endif // LOGIN_H
