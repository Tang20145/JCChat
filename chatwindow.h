#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    void init(QTcpSocket * sock_ref,QString username_ref);
    ~ChatWindow();

private slots:
    void on_sendPushButton_clicked();

private:
    void closeEvent(QCloseEvent* e);

    Ui::ChatWindow *ui;
    QTcpSocket * sock;
    QString username;
};

#endif // CHATWINDOW_H
