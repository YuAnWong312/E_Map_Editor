#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H 1

#include <QMap>
#include <QVector>
#include <QWidget>

#include <Database/Export.h>

class QStandardItemModel;
class QTableView;
class QPushButton;
class QPersistentModelIndex;
class QModelIndex;

class KCSDB;

namespace DataBase
{
class ComboBoxDelegate;
}

namespace DataBase
{

typedef QMap<void*, QPersistentModelIndex*> RecordMap;
typedef QVector<void*> DatabaseNodes;

//class DATABASE_EXPORT DatabaseConfig : public QWidget
class DatabaseConfig : public QWidget
{
    Q_OBJECT
public:
    DatabaseConfig(QWidget *parent = 0);
    ~DatabaseConfig();

    bool initDatabaseConnection();
    bool initDatabaseConnection(const QString& host, const QString& user, const QString& password);

    virtual void initModel();
    virtual void initWidget();
    virtual void save(){}
    virtual void reset();
    virtual void resetMap();

    void setModel(QStandardItemModel* model);
    const RecordMap& getMap();

signals:
    void selectObjectChange(void*);

public slots:
    void ConnectionConfigSlot();

    virtual void addRow(void* point);
    virtual void addRow(const DatabaseNodes& dn);

    void removeRow(void* point);
    void removeRow(const DatabaseNodes& dn);

    void setCurrentSelect(void* point);
    void selectChange(const QModelIndex & current, const QModelIndex & previous);
    void* getSelectObject(int row);

    //test
    void addConnection();
    void removeConnection();

protected:
    QTableView* _tableView;
    QStandardItemModel* _model;
    ComboBoxDelegate* _delegate;
    QPushButton* _connectionConfig;

    RecordMap _map;
    KCSDB* _database;
};

}
#endif
