#ifndef OBJECTSRECORDLISTWIDGET_H
#define OBJECTSRECORDLISTWIDGET_H 1

#include <QListWidget>
#include <MapFramework/MapUtils.h>
#include <MapFramework/Export.h>

namespace MapGUI
{

class MAPGUI_EXPORT ObjectsRecordListWidget : public QListWidget
{
    Q_OBJECT
public:
    ObjectsRecordListWidget(QWidget *parent = 0);

    virtual void init();
    virtual QSize sizeHint() const;

    QListWidgetItem* addItemToMe(const QString& name);

public slots:
    void addItemsToMe(Items&);
    void deleteItems(Items&);
    void reset();

signals:
    void signalSelectionChange(QListWidgetItem*);

protected:
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);

};

}
#endif // OBJECTSRECORDLISTWIDGET_H
