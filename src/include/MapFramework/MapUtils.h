
#ifndef MAPUTILS_H
#define MAPUTILS_H 1

class QListWidgetItem;
class QString;

#include <QVector>
#include <MapFramework/Export.h>

namespace MapGUI
{
typedef QVector<QListWidgetItem*> Items;
extern MAPGUI_EXPORT QListWidgetItem* createItem(const QString& str);
}

#endif
