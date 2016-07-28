
#include <Database/ConnectionConfig.h>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExpValidator>

using namespace DataBase;

ConnectionConfig::ConnectionConfig(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    InitWidget();
}

void ConnectionConfig::InitWidget()
{
    _connect = false;

    setWindowTitle("���ݿ���������");

    QLabel* label_host = new QLabel("������");
    _host = new QLineEdit("192.168.4.22");
    QRegExp rx ("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    QValidator *validator = new QRegExpValidator(rx, this);
    _host->setValidator(validator);
    //_host->setInputMask("000.000.000.000;");

    QLabel* label_user = new QLabel("�û���");
    _user = new QLineEdit("kdf");

    QLabel* label_password = new QLabel("��֤��");
    _password = new QLineEdit("keyvia");

    _connectButton = new QPushButton("����");
    _disconnectButton = new QPushButton("�Ͽ�");

    QGridLayout* mainLayout = new QGridLayout;
    this->setLayout(mainLayout);
    mainLayout->addWidget(label_host, 0, 0);
    mainLayout->addWidget(_host, 0, 1, 1, 2);
    mainLayout->addWidget(label_user, 1, 0);
    mainLayout->addWidget(_user, 1, 1, 1, 2);
    mainLayout->addWidget(label_password, 2, 0);
    mainLayout->addWidget(_password, 2, 1, 1, 2);

    mainLayout->addWidget(_connectButton, 3, 0);
    mainLayout->addWidget(_disconnectButton, 3, 2);

    QObject::connect(_connectButton, SIGNAL(clicked()), this ,SLOT(connectSlot()));
    QObject::connect(_disconnectButton, SIGNAL(clicked()), this ,SLOT(disconnectSlot()));
}

void ConnectionConfig::connectSlot()
{
    _connect = true;
    this->accept();
}

void ConnectionConfig::disconnectSlot()
{
    _connect = false;
    this->accept();
}

void ConnectionConfig::getMessageFromMe(QString& host, QString& user, QString& password, bool& connect)
{
    host = _host->text();
    user = _user->text();
    password = _password->text();
    connect = _connect;
}
