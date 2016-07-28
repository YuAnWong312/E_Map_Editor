
#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H 1

#include <QDialog>
#include <Database/Export.h>

class QLineEdit;
class QPushButton;

namespace DataBase
{
//class DATABASE_EXPORT ConnectionConfig : public QDialog
class ConnectionConfig : public QDialog
{
    Q_OBJECT
public:
    ConnectionConfig(QWidget * parent = 0, Qt::WindowFlags f = 0);

    void InitWidget();
    void getMessageFromMe(QString& host, QString& user, QString& password, bool& connect);

public slots:
    void connectSlot();
    void disconnectSlot();

protected:

    QLineEdit* _host;
    QLineEdit* _user;
    QLineEdit* _password;

    QPushButton* _connectButton;
    QPushButton* _disconnectButton;

    bool _connect;
};
}

#endif // CONNECTIONCONFIG_H
