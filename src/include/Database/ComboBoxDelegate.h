
#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H 1

#include <QItemDelegate>
#include <Database/Export.h>

class KCSDB;

namespace DataBase
{
//class DATABASE_EXPORT ComboBoxDelegate : public QItemDelegate
class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setDatabase(KCSDB* db);

protected:

    KCSDB* _database;
    QString _defaultShow;

};

}

#endif // COMBOBOXDELEGATE_H
