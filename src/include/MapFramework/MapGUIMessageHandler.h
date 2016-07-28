#ifndef MAPGUIMESSAGEHANDLER_H
#define MAPGUIMESSAGEHANDLER_H 1

#include <osgScene/MessageHandler.h>
#include <osgScene/SceneUtils.h>
#include <MapFramework/MapUtils.h>

#include <QObject>
#include <QMap>

class QListWidgetItem;

namespace MapGUI
{
typedef std::vector< QString > StringList;
typedef QMap<osg::observer_ptr<osg::MatrixTransform>, QListWidgetItem* > RecordMap;

class MAPGUI_EXPORT MapGUIMessageHandler : public QObject, public osgScene::MessageHandler
{
    Q_OBJECT
public:
    MapGUIMessageHandler(QObject* parent = 0);
    static MapGUIMessageHandler* instance();
    //reimplement
    virtual bool addNodeToScene(osg::MatrixTransform*);
    virtual bool addNodesToScene(const osgScene::NodeList&);

    virtual bool deleteNodeFromScene(osg::MatrixTransform*);
    virtual bool deleteNodesFromScene(const osgScene::NodeList&);

    virtual void newScene();
    virtual void reset();

public:
    virtual void addNodeItemPair(osg::MatrixTransform* node, QListWidgetItem* item);
    virtual void addNodeItemPair(osg::MatrixTransform* node, const QString& str);

    virtual void deleteSelectionNodes();
    virtual void setCurrentSelectionNodes(const osgScene::SelectionList& sl);
    const osgScene::SelectionList getCurrentSelections();

    virtual void addPositionChangeToCommandBuffer(const QString& str);
    void addCommand(unsigned int i);

    bool openFile(const std::string &path);
    osg::MatrixTransform* addNode(const std::string& path, const std::string& name, int type, osg::Node::NodeMask mask, const QString& itemName);
    int duplicateNode();

    void getSelections(osgScene::NodeList& nl, bool clearAfterCopy = true);

protected:
    virtual bool handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual bool handleCommandBuffer();
    virtual void handleDraggerMessage();
    virtual bool handleNewScene();
    virtual bool handleDragFinish();
    virtual bool handleSelectionsChange();

public slots:
    void slotSelectionItemChange(QListWidgetItem* item);
    void slotPositionInfoChange(QString str);
    void slotMoveFinish();

signals:
    void signalAddListWidgetItem(Items& itemAdd);
    void signalDeleteListWidgetItem(Items& itemDelete);
    void signalAddDatabaseItem(void*);
    void signalDeleteDatabaseItem(void*);
    void signalReset();
    void signalPositionInfo(QString str);

protected:
    RecordMap _recordMap;

    osgScene::SelectionList _currentSelectionNodes;
    mutable OpenThreads::Atomic _selectionNodesChange;
    OpenThreads::Mutex _selectionNodesChangeMutex;

    mutable OpenThreads::Atomic _dragFinish;

    StringList _positionChangeCommandBuffer;

    OpenThreads::Mutex _draggerChangeMutex;
    std::vector<unsigned int> _commands;

    osgScene::NodeList _sl;


};

}
#endif
