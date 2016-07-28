
#include <MapFramework/ToolList.h>
#include <QtXml/QDomDocument>

using namespace MapGUI;

ItemInfo::ItemInfo()
{
    _index = 0;
    _type = NOT_HAVE_DATABASE_INFO;
    _mask = 0xffffffff;
    _name = "";
    _imagePath = "";
    _modelPath = "";
}

ItemInfo::ItemInfo(int index, TYPE type, unsigned int mask, QString name, QString imagepath, QString modelpath)
{
    _index = index;
    _type = type;
    _mask = mask;
    _name = name;
    _imagePath = imagepath;
    _modelPath = modelpath;
}

ItemInfo& ItemInfo::operator =(const ItemInfo& a)
{
    _index = a._index;
    _type = a._type;
    _mask = a._mask;
    _name = a._name;
    _imagePath = a._imagePath;
    _modelPath = a._modelPath;

    return *this;
}

void ItemInfo::print()
{
    qDebug()<<"type="      <<_type     <<""
            <<"mask="      <<_mask     <<""
            <<"name="      <<_name     <<""
            <<"imagePath=" <<_imagePath<<""
            <<"modelPath"  <<_modelPath;
}

QString ItemInfo::getString()
{
    QString str = QString::number(_type) + "," + QString::number(_mask) + "," + _name + "," + _modelPath;

    return str;
}

void ItemInfo::parseString(QString &str)
{
    QStringList strlist = str.split(",", QString::SkipEmptyParts);

    _type = strlist.at(0).toInt();
    _mask = 0x1<<strlist.at(1).toInt();
    _name = strlist.at(2);
    _modelPath = strlist.at(3);
}

const int W_ICONSIZE = 30;			//图片宽度
const int H_ICONSIZE = 30;			//图片高度
const int H_ITEMSIZE = W_ICONSIZE;	//单元项高度(因为单元项既包括图片又包括文本)
const int W_WIDGET = 190;			//widget width
const int H_WIDGET = 300;			//
const int SPACE = 5;				//间隔


DragList::DragList(QListWidgetItem* item, QWidget *parent /* = 0 */)
    : QListWidget(parent)
{
	_item = item;
    //this->setWindowFlags(this->windowFlags()&~Qt::FramelessWindowHint);
    this->setViewMode(QListView::IconMode);
    this->setResizeMode(QListView::Adjust);
    this->setMovement(QListView::Static);
    this->setSpacing(SPACE);
    this->setIconSize(QSize(W_ICONSIZE, H_ITEMSIZE));
    //initInfoMap();
    //initItems();
}

DragList::~DragList()
{

}

QSize DragList::sizeHint() const
{
	return QSize(W_WIDGET, H_WIDGET);
}

void DragList::addItemInfo(ItemInfo &info)
{
    QPixmap objPixmap(info._imagePath);
	
    QListWidgetItem* newItem =
            new QListWidgetItem(QIcon(info._imagePath)/*objPixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))*/, ""/*info._name*/, this);
	newItem->setToolTip(info._name);
    newItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));

    QVariant data;
    data.setValue(info);
    newItem->setData(Qt::UserRole, data);

    this->addItem(newItem);
}

/*
void DragList::initInfoMap()
{
    _infoMap[0] =
        ItemInfo(0, ItemInfo::MODAL, 0x1, tr("yicenglou"), "./images/scale.png", "./model/一层楼.3DS");

    _infoMap[1] =
        ItemInfo(1, ItemInfo::MODAL, 0x1<<1, tr("liangcenglou"), "./images/scale.png", "./model/两层楼.3DS");

    _infoMap[2] =
        ItemInfo(2, ItemInfo::EDIT, 0x1<<2, tr("hongwai"), "./images/move.png", "./model/hongwai.3DS");

    _infoMap[3] =
        ItemInfo(3, ItemInfo::EDIT, 0x1<<3, tr("yangan"), "./images/rotate.png", "./model/yangan.3DS");

}

void DragList::initItems()
{

    InfoMap::iterator iter = _infoMap.begin();

    for (iter; iter!=_infoMap.end(); iter++)
    {
        ItemInfo item = iter->second;

        QListWidgetItem* newItem =
            new QListWidgetItem(QIcon(item._imagePath), item._name, this);

        this->addItem(newItem);
    }

}

void DragList::initItems(QString &file)
{

}

ItemInfo& DragList::getItemInfo(int row)
{
    return _infoMap[row];
}
*/

void DragList::startDrag()
{
    QListWidgetItem *item = currentItem();
    if (item)
    {
        QMimeData *mimeData = new QMimeData;
        ItemInfo data = item->data(Qt::UserRole).value<ItemInfo>();
        //data.print();
        QString str = data.getString();
        mimeData->setText(str);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction | Qt::MoveAction);
        //delete drag;
    }
}

void DragList::mousePressEvent( QMouseEvent *event )
{
    if (event->button() == Qt::LeftButton)
        startPos = event->pos();
    QListWidget::mousePressEvent(event);
}

void DragList::mouseMoveEvent( QMouseEvent *event )
{
    if (event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag();
		
		//this->itemAt(mapFromGlobal(QCursor::pos())
    }
    QListWidget::mouseMoveEvent(event);
}

void DragList::resizeEvent(QResizeEvent* event)
{
	QListWidget::resizeEvent(event);
	QSize size = this->size();
	//_item->setSizeHint(size);
}
/*
void DragList::dropEvent( QDropEvent *event )
{
	osg::notify(osg::FATAL)<<"22drop"<<std::endl;
    DragList *source =
        qobject_cast<DragList *>(event->source());
    if (source && source == this)
    {
        qDebug()<<event->mimeData()->text();//////////////////

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}

void DragList::dragEnterEvent( QDragEnterEvent *event )
{
	osg::notify(osg::FATAL)<<"22enter"<<std::endl;
    DragList *source =
        qobject_cast<DragList *>(event->source());
    if (source && source == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}

void DragList::dragMoveEvent( QDragMoveEvent *event )
{
	osg::notify(osg::FATAL)<<"22move"<<std::endl;
    DragList *source =
        qobject_cast<DragList *>(event->source());
    if (source && source == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}
*/

/*
void DragList::contextMenuEvent( QContextMenuEvent * event )
{

    QMenu* popMenu = new QMenu(this);
    popMenu->addAction(new QAction(tr("delete"), this));
    popMenu->addAction(new QAction(tr("add"), this));

    if(this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {

    }
    popMenu->exec(QCursor::pos());

}


void ToolList::removeTool(Tool *tool)
{
   // Tools::iterator itr =
   // _toolRecord.erase()
}
*/

Tool::Tool(QListWidget *parent, QString buttonName) : QObject(parent)
{
    if(parent)
    {
        button_size = QSize(0,20);
        list_size = QSize(0,150);

        button_item = new QListWidgetItem(parent);
        list_item = new QListWidgetItem(parent);

		button = new QPushButton(buttonName, parent);
		list = new DragList(list_item, parent);

        button_item->setSizeHint(button_size);
        //list_item->setSizeHint(list_size);

        parent->addItem(button_item);
        parent->addItem(list_item);
        parent->setItemWidget(button_item, button);
        parent->setItemWidget(list_item, list);

        QObject::connect( button, SIGNAL(clicked()), this, SLOT(button_push()) );
    }
}

void Tool::button_push()
{
    bool hide = list->isHidden();

    if(!hide)
    {
        list_item->setSizeHint(QSize(0,0));
    }
    else
    {
        list_item->setSizeHint(list_size);
    }
    return;
}

void Tool::addItem(ItemInfo &info)
{
    list->addItemInfo(info);
}

void Tool::addFinish()
{
	int num = list->count();
	int width = W_ICONSIZE + SPACE;
	int height = H_ICONSIZE + SPACE;
	int row = num>( W_WIDGET/width - 1 ) ? ( num/(W_WIDGET/width - 1) + 1 ):1;
	height = (row+1)*height;

	list_size.setHeight(height);
	list_item->setSizeHint(list_size);
}

ToolList::ToolList(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	this->setAcceptDrops(true);

    _topList = new QListWidget;
	
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_topList);
    this->setLayout(layout);

    QString str = QString("./images/ToolBox.txt");
    initFile(str);

    //this->resize(150,300);
}

ToolList::~ToolList()
{
    _toolRecord.clear();
}

void ToolList::initFile(QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<< "Error: Cannot read file";
        return;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;

    if(!doc.setContent(&file,true,&errorStr,&errorLine,&errorColumn))
    {
        qDebug()<< "Error: Prase error at line ";
        file.close();
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode TYPE = root.firstChild();//TYPE
    qDebug("%i", root.childNodes().count());

    while(!TYPE.isNull())
    {
        QDomElement tmp= TYPE.toElement();
        if(!tmp.isNull())
        {
            qDebug() << tmp.tagName() << ":" << tmp.attribute("name");

            Tool* tool = new Tool(_topList, tmp.attribute("name"));
            this->addTool(tool);

            QDomNode Item = tmp.firstChild();
            while(!Item.isNull())
            {
                QDomElement n= Item.toElement();
                if(!n.isNull())
                {
                    qDebug() << n.tagName();

                    ItemInfo info;

                    QDomNode child = n.firstChild();
                    if(!child.isNull())
                    {
                        QDomElement content = child.toElement();
                        qDebug() << content.tagName()<<":"<<content.text();
                        info._type = content.text().toInt();
                        child = child.nextSibling();
                    }
                    if(!child.isNull())
                    {
                        QDomElement content = child.toElement();
                        qDebug() << content.tagName()<<":"<<content.text();
						bool ok = false;
                        info._mask = content.text().toUInt(&ok, 10);
                        child = child.nextSibling();
                    }
                    if(!child.isNull())
                    {
                        QDomElement content = child.toElement();
                        qDebug() << content.tagName()<<":"<<content.text();
                        info._name = content.text();
                        child = child.nextSibling();
                    }
                    if(!child.isNull())
                    {
                        QDomElement content = child.toElement();
                        qDebug() << content.tagName()<<":"<<content.text();
                        info._imagePath = content.text();
                        child = child.nextSibling();
                    }
                    if(!child.isNull())
                    {
                        QDomElement content = child.toElement();
                        qDebug() << content.tagName()<<":"<<content.text();
                        info._modelPath = content.text();
                        child = child.nextSibling();
                    }

                    tool->addItem(info);
                }
                Item = Item.nextSibling();
            }

			tool->addFinish();
        }

        TYPE = TYPE.nextSibling();
    }
}

void ToolList::addTool(Tool *tool)
{
    _toolRecord.push_back(tool);
}


QSize ToolList::sizeHint() const
{
	return QSize(W_WIDGET, H_WIDGET);
}

void ToolList::dropEvent( QDropEvent *event )
{
	/*
	DragList *source =
		qobject_cast<DragList *>(event->source());
    if (source && source == this)
	{
		qDebug()<<event->mimeData()->text();//////////////////

		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	*/
	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}

void ToolList::dragEnterEvent( QDragEnterEvent *event )
{
	event->setDropAction(Qt::MoveAction);
	event->acceptProposedAction();
}
