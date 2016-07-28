#ifndef TOOLLIST_H
#define TOOLLIST_H 1

#include <QtGui>
#include <QVector>
#include <MapFramework/Export.h>

namespace MapGUI
{

class MAPGUI_EXPORT ItemInfo
{
public:
    enum TYPE
    {
		NOT_HAVE_DATABASE_INFO  = 0,
        HAVE_DATABASE_INFO = 1
    };

    ItemInfo();
    ItemInfo(int index, TYPE type, unsigned int mask, QString name,
        QString imagepath, QString modelpath);

    ItemInfo& operator=(const ItemInfo& a);

    void print();
    QString getString();
    void parseString(QString& str);

    int _index;
    int _type;
    unsigned int _mask;
    QString _name;
    QString _imagePath;
    QString _modelPath;
};

class  MAPGUI_EXPORT DragList : public QListWidget
{
    Q_OBJECT

public:
    DragList(QListWidgetItem* item, QWidget *parent = 0);
    ~DragList();

	virtual QSize sizeHint() const;

    void addItemInfo(ItemInfo& info);

    //void initInfoMap();
    //void initItems();
    //ItemInfo& getItemInfo(int row);
    //InfoMap _infoMap;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent* event);

	//void contextMenuEvent(QContextMenuEvent * event);
    //void dragEnterEvent(QDragEnterEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    //void dropEvent(QDropEvent *event);
	
protected:
    void startDrag();
    QPoint startPos;
	QListWidgetItem* _item;
};

class MAPGUI_EXPORT Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QListWidget* parent, QString buttonName);
    ~Tool(){}

    void addItem(ItemInfo& info);
	void addFinish();

public slots:
    void button_push();

protected:
    DragList* list;
    QPushButton* button;

    QListWidgetItem* list_item;
    QListWidgetItem* button_item;

    QSize button_size;
    QSize list_size;

};

class MAPGUI_EXPORT ToolList : public QWidget
{
    Q_OBJECT
public:
    ToolList(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~ToolList();

    void initFile(QString& fileName);
    void addTool(Tool* tool);

	virtual QSize sizeHint() const;
   // void removeTool(Tool* tool);

    typedef QVector<Tool*> Tools;

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	//void dragMoveEvent(QDragMoveEvent *event);

protected:
    QListWidget* _topList;

    Tools _toolRecord;

};

}

Q_DECLARE_METATYPE(MapGUI::ItemInfo)

#endif // TOOLLIST_H
