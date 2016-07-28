
#include <QListWidgetItem>

#include <MapFramework/ObjectsRecordListWidget.h>

#include <QDragEnterEvent>

using namespace MapGUI;

ObjectsRecordListWidget::ObjectsRecordListWidget(QWidget *parent)
	: QListWidget(parent)
{
    init();
}

void ObjectsRecordListWidget::init()
{
    QObject::connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SIGNAL(signalSelectionChange(QListWidgetItem*)));

    this->setAcceptDrops(true);

    /*
    QObject::connect(_copy, SIGNAL(triggered()), this, SLOT(slotCopy()));
    QObject::connect(_paste, SIGNAL(triggered()), this, SLOT(slotPaste()));
    QObject::connect(_duplicate, SIGNAL(triggered()), this, SLOT(slotDuplicate()));
    QObject::connect(_rename, SIGNAL(triggered()), this, SLOT(slotRename()));
    QObject::connect(_delete, SIGNAL(triggered()), this, SLOT(slotDelete()));
    */
}

/*
void ObjectsRecordListWidget::convertSignal( QListWidgetItem* item )
{

	osg::MatrixTransform* mt = ResourceManager::instance()->findMatrixNode(item);
	if(mt)
	{
		ResourceManager::instance()->setCurrentItem(item);

	}
	else
	{
		ResourceManager::instance()->setCurrentItem(0);
	}
	emit emitMT(mt);
}
*/

/*
void ObjectsRecordListWidget::contextMenuEvent( QContextMenuEvent * event )
{

	if(this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
	{
		_popMenu->exec(QCursor::pos());
	}

}
*/

QSize ObjectsRecordListWidget::sizeHint() const
{
    return QSize(190, 280);
}

QListWidgetItem* ObjectsRecordListWidget::addItemToMe(const QString &name)
{
    QListWidgetItem* newItem = new QListWidgetItem(name, this);
    newItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return newItem;
}

void ObjectsRecordListWidget::addItemsToMe(Items& items)
{
    Items::iterator itr = items.begin();

    for (itr; itr != items.end(); itr++)
    {
        QListWidgetItem* item = *itr;
        if (item)
        {
            addItem(item);
        }
    }
}

void ObjectsRecordListWidget::deleteItems(Items& items)
{
    Items::iterator itr = items.begin();

    for (itr; itr != items.end(); itr++)
	{
		QListWidgetItem* item = *itr;
		if (item)
		{
			removeItemWidget(item);
            delete item;
		}
	}
}

void ObjectsRecordListWidget::reset()
{
    if (count() != 0)
    {
        clear();
    }
}

void ObjectsRecordListWidget::dropEvent( QDropEvent *event )
{
	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}

void ObjectsRecordListWidget::dragEnterEvent( QDragEnterEvent *event )
{
	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}
