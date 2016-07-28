
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QPersistentModelIndex>

#include <Database/DatabaseConfig.h>
#include <Database/ConnectionConfig.h>
#include <Database/ComboBoxDelegate.h>

#include <kdfdb.h>

#include <QToolBar>
#include <QToolButton>

using namespace DataBase;

DatabaseConfig::DatabaseConfig(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initModel();
}

DatabaseConfig::~DatabaseConfig()
{
    _database->DBRemoveConnection();
    resetMap();

    if(_database)
        delete _database;
    _database = 0;
}

bool DatabaseConfig::initDatabaseConnection()
{
    /*
    _currentLineID = INT_MIN;
    _currentStationID = INT_MIN;
    _currentDeviceID = INT_MIN;
    */

    int success = _database->DBCreatConnection("192.168.4.22", "kdf", "keyvia");
    if(success == NO_ERR)
    {
        qDebug("database connection is ok");
        return true;
    }
    else
        qDebug("database connection is bad");

    return false;
}

bool DatabaseConfig::initDatabaseConnection(const QString& host, const QString& user, const QString& password)
{
    /*
    _currentLineID = INT_MIN;
    _currentStationID = INT_MIN;
    _currentDeviceID = INT_MIN;
    */

    int success = _database->DBCreatConnection(host, user, password);
    if(success == NO_ERR)
    {
        qDebug("database connection is ok");
        return true;
    }
    else
        qDebug("database connection is bad");

    return false;
}

void DatabaseConfig::initModel()
{
    if(_model)
    {
        _model->setColumnCount(3);
        QStringList sl;
        sl<<"线路"<<"站点"<<"编号"<<"输入编号";
        _model->setHorizontalHeaderLabels(sl);
    }
}

void DatabaseConfig::initWidget()
{
    _model = new QStandardItemModel(this);

    _tableView = new QTableView;
    _tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _tableView->setModel(_model);

    QHeaderView* header_H = new QHeaderView(Qt::Horizontal, this);
    header_H->setResizeMode(QHeaderView::Interactive);
    //header_H->setClickable(false);
    _tableView->setHorizontalHeader(header_H);

    _delegate = new ComboBoxDelegate(this);
    _tableView->setItemDelegate(_delegate);

    _database = new KCSDB;
    _delegate->setDatabase(_database);

    /*
    QHeaderView* header_V = new QHeaderView(Qt::Horizontal, this);
    header_V->setClickable(false);
    _tableView->setVerticalHeader(header_V);
    */

    /*
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            QModelIndex index = _model->index(row, column, QModelIndex());
            //_model->setData(index, QVariant((row+1) * (column+1)), Qt::DisplayRole);
            _model->setData(index, INT_MIN, Qt::UserRole);
        }
    }
    */

    _connectionConfig = new QPushButton("连接配置");

    QGridLayout* infoLayout = new QGridLayout;
    infoLayout->addWidget(_tableView, 0, 0, 3, 3);
    infoLayout->addWidget(_connectionConfig, 3, 0, 1, 1);

    QGroupBox *infoBox = new QGroupBox("设备信息");
    infoBox->setLayout(infoLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setMargin(1);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(infoBox);
    this->setLayout(mainLayout);

    QObject::connect( _connectionConfig, SIGNAL(clicked()), this, SLOT(ConnectionConfigSlot()) );
    QObject::connect(_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                     this, SLOT(selectChange(QModelIndex,QModelIndex)));

    ///////////////////////////////////
    QToolButton* m_add_button=new QToolButton;
    QToolButton* m_remove_button=new QToolButton;

    QToolBar *toolBar = new QToolBar;
    toolBar->setIconSize(QSize(22, 22));
    connect(m_add_button, SIGNAL(clicked()), this, SLOT(addConnection()));
    toolBar->addWidget(m_add_button);

    connect(m_remove_button, SIGNAL(clicked()), this, SLOT(removeConnection()));
    toolBar->addWidget(m_remove_button);

    mainLayout->addWidget(toolBar);
    ///////////////////////////////////
}

static int name = 0;
typedef QMap<QString, QGroupBox*> tmpMap;
tmpMap map;

void DatabaseConfig::addConnection()
{
    name++;
    QGroupBox* tmp = new QGroupBox(this);
    QString str = QString("name-%1").arg(name);
    tmp->setObjectName(str);
    addRow(tmp);

    map[str] = tmp;

}

void DatabaseConfig::removeConnection()
{
    //QGroupBox* child = this->findChild<QGroupBox *>(QString("%1").arg(name));
    QString str = QString("name-%1").arg(name);
    QGroupBox* child = map[str];
    qDebug()<<"remove child name="<<str;
    removeRow(child);
    name--;
}

void DatabaseConfig::reset()
{
    resetMap();
    _model->clear();
    initModel();
}

void DatabaseConfig::resetMap()
{
    RecordMap::Iterator itr = _map.begin();
    for( itr; itr != _map.end(); itr++ )
    {
        QPersistentModelIndex* storeIndex = itr.value();
        delete storeIndex;
    }
    _map.clear();
}

void DatabaseConfig::setModel(QStandardItemModel *model)
{
    if(model != 0 && _model != model)
    {
        resetMap();
        _tableView->setModel(model);
        _model->deleteLater();

        _model = model;
    }
}

const RecordMap& DatabaseConfig::getMap()
{
    return _map;
}

void DatabaseConfig::ConnectionConfigSlot()
{
    ConnectionConfig cf(this);
    if( QDialog::Accepted == cf.exec())
    {
        QString host;
        QString user;
        QString password;
        bool connect = false;
        cf.getMessageFromMe(host, user, password, connect);

        if(connect)
        {
            bool success = initDatabaseConnection(host, user, password);
            //may need show messagebox
        }
        else
        {
            _database->DBRemoveConnection();
        }
    }
}

void DatabaseConfig::addRow(void *point)
{
    bool success = _model->insertRow(_model->rowCount());
    if(success)
    {
        QModelIndex tmpIndex = _model->index(_model->rowCount()-1, 0);
        _model->setData(tmpIndex, INT_MIN, Qt::UserRole);
        QPersistentModelIndex* storeIndex = new QPersistentModelIndex(tmpIndex);
        qDebug()<<"add row="<<storeIndex->row();
        _map.insert(point, storeIndex);

        //_model->setVerticalHeaderItem(_model->rowCount()-1, new QStandardItem("OK"));
    }
}

void DatabaseConfig::addRow(const DatabaseNodes &dn)
{
    DatabaseNodes dbn = dn;
}

void DatabaseConfig::removeRow(void *point)
{
    RecordMap::iterator itr = _map.find(point);
    if(itr != _map.end())
    {
        qDebug()<<"find child";
        QPersistentModelIndex* storeIndex = itr.value();
        if(storeIndex->isValid())
        {
            qDebug()<<"remove row="<<storeIndex->row();
            _model->removeRow(storeIndex->row());
            delete storeIndex;
            _map.erase(itr);
        }
    }
}

void DatabaseConfig::removeRow(const DatabaseNodes &dn)
{
    DatabaseNodes dbn = dn;

}

void DatabaseConfig::setCurrentSelect(void *point)
{
    RecordMap::Iterator itr = _map.find(point);
    if(itr != _map.end())
    {
        QPersistentModelIndex* storeIndex = itr.value();
        if(storeIndex->isValid())
        {
            _tableView->selectRow(storeIndex->row());
        }
    }
}

void* DatabaseConfig::getSelectObject(int row)
{
    void* select = 0;
    for(RecordMap::Iterator itr = _map.begin(); itr != _map.end(); itr++)
    {
        QPersistentModelIndex* storeIndex = itr.value();
        if(storeIndex->isValid() && row == storeIndex->row())
        {
            select = itr.key();
        }
    }
    return select;
}

void DatabaseConfig::selectChange(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    int row = current.row();
    void* select = getSelectObject(row);

    ///////
    //QGroupBox* tmp = (QGroupBox*)select;
    //if(tmp)
    //    qDebug()<<"select object name="<<tmp->objectName();
    ///////

    emit selectObjectChange(select);
}
