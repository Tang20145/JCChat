#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include "chatwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_connectPushButton_clicked();

private:
    Ui::LoginWindow *ui;
    QTcpSocket * sock;
    ChatWindow * chatwindow;
};
#endif // LOGINWINDOW_H
