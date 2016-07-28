
#include <MapFramework/MapUtils.h>
#include <QString>
#include <QListWidgetItem>

namespace MapGUI
{
QListWidgetItem* createItem(const QString &str)
{
    QListWidgetItem* item = new QListWidgetItem(str);
    //item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}
}

