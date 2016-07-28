
#include <MapFramework/MapGUIMessageHandler.h>
#include <MapFramework/MapDragger.h>
#include <MapFramework/CollectVisitor.h>
#include <osgDragger/ShowBound>
#include <osg/ValueObject>
#include <osgDB/ReadFile>

#include <QDebug>
#include <QListWidgetItem>

namespace Tool
{

void computeRotate(const osg::Matrix& mt, float& p, float& h, float& b)
{
    float m11,m12,m13;
    float m21,m22,m23;
    float m31,m32,m33;

    m11 = mt.ptr()[0]; m12 = mt.ptr()[1]; m13 = mt.ptr()[2];
    m21 = mt.ptr()[4]; m22 = mt.ptr()[5]; m23 = mt.ptr()[6];
    m31 = mt.ptr()[8]; m32 = mt.ptr()[9]; m33 = mt.ptr()[10];

    p = asin(-m32);
    h = atan2(m31, m33);
    b = atan2(m12, m22);
}

QString computePosition(osg::MatrixTransform* MT)
{
    QString str = "";

    osg::Matrix mt = osgScene::getWorldMatrix(MT);
    osg::Vec3 trans = mt.getTrans();
    str = str + QString("%1").arg(trans.x(),0,'f',1) + ";";
    str = str + QString("%1").arg(trans.y(),0,'f',1) + ";";
    str = str + QString("%1").arg(trans.z(),0,'f',1) + ";";

    float x=0.0f;
    float y=0.0f;
    float z=0.0f;
    computeRotate(mt, x, y, z);
    str = str + QString("%1").arg(osg::RadiansToDegrees(x),0,'f',1) + ";";
    str = str + QString("%1").arg(osg::RadiansToDegrees(y),0,'f',1) + ";";
    str = str + QString("%1").arg(osg::RadiansToDegrees(z),0,'f',1);
    return str;
}

}

using namespace MapGUI;
MapGUIMessageHandler::MapGUIMessageHandler(QObject *parent)
    : QObject(parent) , _selectionNodesChange(0) , _dragFinish(0)
{

}

static osg::ref_ptr<MapGUIMessageHandler> rh = 0;
MapGUIMessageHandler* MapGUIMessageHandler::instance()
{
    if(!rh.valid())
    {
        rh = new MapGUIMessageHandler;
    }
    return rh.get();
}

bool MapGUIMessageHandler::addNodeToScene(osg::MatrixTransform *node)
{
    MessageHandler::addNodeToScene(node);
    return false;
}

bool MapGUIMessageHandler::addNodesToScene(const osgScene::NodeList &nv)
{
    MessageHandler::addNodesToScene(nv);

    osgScene::SelectionList sl;
    for(osgScene::NodeList::const_iterator itr = nv.begin(); itr != nv.end(); itr++)
    {
        sl.push_back( itr->get() );
    }
    setCurrentSelectionNodes(sl);
    return false;
}

bool MapGUIMessageHandler::deleteNodeFromScene(osg::MatrixTransform *node)
{
    MessageHandler::deleteNodeFromScene(node);
    return false;
}

bool MapGUIMessageHandler::deleteNodesFromScene(const osgScene::NodeList &nv)
{
    /*
    SelectionList sl;
    for(NodeVector::const_iterator itr = nv.begin(); itr != nv.end(); itr++)
    {
        sl.push_back( itr->get() );
    }
    */

    MessageHandler::deleteNodesFromScene(nv);
    return false;
}

void MapGUIMessageHandler::newScene()
{
    reset();
    MessageHandler::newScene();
}

void MapGUIMessageHandler::reset()
{
    _recordMap.clear();
    _sl.clear();
    // gui thread
    emit signalReset();
}

void MapGUIMessageHandler::addNodeItemPair(osg::MatrixTransform *node, QListWidgetItem *item)
{
    _recordMap[node] = item;
    //gui thread
}

void MapGUIMessageHandler::addNodeItemPair(osg::MatrixTransform *node, const QString &str)
{
    QListWidgetItem* item = createItem(str);
    _recordMap[node] = item;

    Items items;
    items.push_back(item);
    emit signalAddListWidgetItem(items);
}

void MapGUIMessageHandler::deleteSelectionNodes()
{
    SelectionList sl;
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
        sl.swap(_currentSelectionNodes);
    }

    SelectionList::iterator itr = sl.begin();
    osgScene::NodeList nv;
    Items items;
    for(itr; itr != sl.end(); itr++)
    {
        //osg::MatrixTransform* node = itr->get();
        osg::ref_ptr<osg::MatrixTransform> node = 0;
        if(itr->lock(node))
        {
            nv.push_back( node.get() );
            items.push_back( _recordMap.value(node.get(), 0) );
            _recordMap.remove(node.get());

            emit signalDeleteDatabaseItem(node.get());
        }
    }

    qDebug()<<"Record Map size="<<_recordMap.size();
    emit signalDeleteListWidgetItem(items);
    this->deleteNodesFromScene(nv);

    {
        SelectionList sl;
        this->setCurrentSelectionNodes(sl);
    }
}

void MapGUIMessageHandler::setCurrentSelectionNodes(const osgScene::SelectionList& sl)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
    _currentSelectionNodes = sl;

    ++_selectionNodesChange;
}

const osgScene::SelectionList MapGUIMessageHandler::getCurrentSelections()
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
    return _currentSelectionNodes;
}

void MapGUIMessageHandler::addPositionChangeToCommandBuffer(const QString &str)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_commandBufferMutex);
    _positionChangeCommandBuffer.push_back(str);
}

void MapGUIMessageHandler::addCommand(unsigned int i)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_draggerChangeMutex);
    _commands.push_back(i);
}

bool MapGUIMessageHandler::openFile(const std::string &path)
{
    if (!_root.valid())
    {
        return false;
    }

    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);

    if(!node.valid())
    {
        return false;
    }
    else
    {
        newScene();

        CollectVisitor visitor;
        node->accept(visitor);
        visitor.getSelections(_sl, true);
        std::cout<<"open file object size ="<<_sl.size()<<std::endl;

        osgScene::NodeList& nl = _sl;
        for (osgScene::NodeList::iterator itr = nl.begin(); itr != nl.end(); itr++)
        {
            std::string name = "undefined";
            itr->get()->getUserValue<std::string>("NAME", name);
            addNodeItemPair(itr->get(), QString::fromStdString(name));

            emit signalAddDatabaseItem(itr->get());
        }

        addNodesToScene(_sl);
        resetCameraPosition();

        return true;
    }
    return false;
}

osg::MatrixTransform* MapGUIMessageHandler::addNode(const std::string &path, const std::string &name,
                                                    int type, osg::Node::NodeMask mask, const QString &itemName)
{
    if (!_root.valid())
    {
        return 0;
    }

    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);

    if (node.valid())
    {
        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
        mt->setUserValue<std::string>("NAME", name);
        mt->setUserValue<int>("TYPE", type);
        mt->setNodeMask(mask);
        mt->addChild(node.get());

        //////////////////////////////////////////////////
        /*
        osg::ref_ptr<osg::LOD> lod = new osg::LOD;
        float radius = node->getBound().radius();
        lod->setRadius(radius);
        lod->setRange( 0, radius * 50.0f, FLT_MAX );
        lod->setRange( 1, 0.0f, radius * 50.0f );
        osg::Vec3 center = node->getBound().center();
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable( new osg::ShapeDrawable(new osg::Box(center, radius*0.3)) );
        lod->insertChild(0, geode.get());
        lod->insertChild(1, node.get());
        mt->addChild(lod.get());
        */
        ///////////////////////////////////////////////////

        addNodeToScene(mt.get());

        SelectionList sl;
        sl.push_back(mt);
        setCurrentSelectionNodes(sl);

        addNodeItemPair(mt, itemName);

        emit signalAddDatabaseItem(mt);
        return mt.get();
    }

    return 0;
}

int MapGUIMessageHandler::duplicateNode()
{
    osgScene::SelectionList sl = getCurrentSelections();

    osgScene::NodeList nl;
    for (osgScene::SelectionList::iterator itr = sl.begin(); itr != sl.end(); itr++)
    {
        osg::ref_ptr<osg::MatrixTransform> preMt = 0;

        if ( (*itr).lock(preMt) )
        {
            osg::ref_ptr<osg::MatrixTransform> mt = 0;

            {
                mt = osgScene::duplicate(preMt.get());
                std::string name = "undefined";
                preMt->getUserValue<std::string>("NAME", name);

                addNodeItemPair(mt.get(), QString::fromStdString(name));
                emit signalAddDatabaseItem(mt.get());
            }
            nl.push_back(mt.get());
        }
    }
    addNodesToScene(nl);
    int size = nl.size();
    return size;
}

void MapGUIMessageHandler::getSelections(osgScene::NodeList &nl, bool clearAfterCopy)
{
    nl = _sl;
    if(clearAfterCopy)
    {
        _sl.clear();
    }
}

bool MapGUIMessageHandler::handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(!_root.valid())
        return false;

    MessageHandler::handleFrame(ea, aa);

    handleSelectionsChange();
    handleDraggerMessage();
    handleDragFinish();
    return false;
}

bool MapGUIMessageHandler::handleCommandBuffer()
{
    StringList sl;
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_commandBufferMutex);
        sl.swap( _positionChangeCommandBuffer );
    }
    if(sl.empty())
        return false;

    SelectionList slNode;
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
        slNode = _currentSelectionNodes;
    }
    if(slNode.empty())
        return false;

    osg::ref_ptr<osg::MatrixTransform> node = slNode.begin()->get();
    if(!node.valid())
        return false;
    /*
    {

        if(slNode.size() != 1 || slNode.size() == 0)
            return false;
        SelectionList::iterator itr = _currentSelectionNodes.begin();
        if( !(*itr).lock(node) )
            return false;
    }
    */

    osg::Matrix mt = osgScene::getWorldMatrix(node.get());

    bool change = false;
    for(StringList::iterator itr = sl.begin(); itr != sl.end(); itr++)
    {
        QString parseStr = *itr;
        if ( !parseStr.isEmpty() )
        {
            QStringList valueList = parseStr.split(";");
            if(valueList.size() == 6)
            {
                change = true;
                float posX = valueList[0].toFloat();
                float posY = valueList[1].toFloat();
                float posZ = valueList[2].toFloat();
                mt.setTrans(posX, posY, posZ);

                float rotX = valueList[3].toFloat();
                float rotY = valueList[4].toFloat();
                float rotZ = valueList[5].toFloat();
                double x = osg::DegreesToRadians(rotX);
                double y = osg::DegreesToRadians(rotY);
                double z = osg::DegreesToRadians(rotZ);
                mt.setRotate(osg::Quat(x, osg::X_AXIS, y, osg::Y_AXIS, z, osg::Z_AXIS));

            }
        }
    }

    if(change)
    {
        node->setMatrix(mt);
        this->setCurrentSelectionNodes(slNode);
    }

    return false;
}

void MapGUIMessageHandler::handleDraggerMessage()
{
    std::vector<unsigned int> localCommand;

    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_draggerChangeMutex);
        localCommand.swap(_commands);
    }

    if (localCommand.size()>0)
    {
        OSG_WARN<<"command = "<<localCommand.size()<<std::endl;
    }

    for(unsigned int i = 0; i < localCommand.size(); i++)
    {
        unsigned int index = localCommand[i];
        if (index == 1)
        {
            CustomerDraggerManager::Instence()->setActiveDragger("move");

        }

        if (index == 2)
        {
            CustomerDraggerManager::Instence()->setActiveDragger("rotate");

        }

        if (index == 3)
        {
            CustomerDraggerManager::Instence()->setActiveDragger("");

        }

    }
}

bool MapGUIMessageHandler::handleNewScene()
{
    int scene = _newScene;
    if( scene>0 )
    {
        osgDragger::ShowBound::Instence()->reset();
        CustomerDraggerManager::Instence()->reset();
    }

    MessageHandler::handleNewScene();
    return true;
}

bool MapGUIMessageHandler::handleDragFinish()
{
    int  dragFinish = _dragFinish;
    if(dragFinish > 0)
    {
        SelectionList sl;
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
            sl = _currentSelectionNodes;
        }

        QString str;
        if(sl.size() == 1)
        {
            osg::ref_ptr<osg::MatrixTransform> node = sl.begin()->get();
            if(!node.valid())
                return false;

            str = Tool::computePosition( node.get() );
        }
        emit signalPositionInfo(str);
        --_dragFinish;
    }

    return false;
}

bool MapGUIMessageHandler::handleSelectionsChange()
{
    int change = _selectionNodesChange;
    if( change>0 )
    {
        SelectionList sl;
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);
            sl = _currentSelectionNodes;
        }

        QString str;
        if(sl.size() == 1)
        {
            if( sl.begin()->valid() )
            {

                str = Tool::computePosition( sl.begin()->get());
            }
            /*
            osg::ref_ptr<osg::MatrixTransform> mt = 0;
            if( (*_currentSelectionNodes.begin()).lock(mt) )
            {
                str = Tool::computePosition(mt.get());
            }
            */
        }
        emit signalPositionInfo(str);

        CustomerDraggerManager::Instence()->setSelections(sl);
        osgDragger::ShowBound::Instence()->setSelections(sl);
        --_selectionNodesChange;;
    }

    return false;
}

void MapGUIMessageHandler::slotSelectionItemChange(QListWidgetItem *item)
{
    SelectionList sl;
    sl.push_back(_recordMap.key(item, 0).get());
    this->setCurrentSelectionNodes(sl);
}

void MapGUIMessageHandler::slotPositionInfoChange(QString str)
{
    this->addPositionChangeToCommandBuffer(str);
}

void MapGUIMessageHandler::slotMoveFinish()
{
    /*
    QString str;
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_selectionNodesChangeMutex);

    if(_currentSelectionNodes.size() == 1)
    {
        osg::ref_ptr<osg::MatrixTransform> mt = 0;
        if( (*_currentSelectionNodes.begin()).lock(mt) )
        {
            str = Tool::computePosition(mt.get());
        }
    }

    ++_selectionNodesChange;

    emit signalPositionInfo(str);
    */

    ++_dragFinish;
}

/*
void MapGUIMessageHandler::removeNodeToItemPair(osg::MatrixTransform *node)
{
    osg::ref_ptr<osg::MatrixTransform> mt = node;
    this->deleteNodeFromScene(mt.get());
    QListWidgetItem* item = _recordMap.value(mt.get(), 0);

    Items items;
    items.push_back(item);
    emit signalDeleteListWidgetItem(items);
}
*/
