
#include <osgScene/MessageHandler.h>
#include <osgViewer/Viewer>

using namespace osgScene;

MessageHandler::MessageHandler() : _root(0), _newScene(0), _resetCameraPosition(0)
{
}

/*
static osg::ref_ptr<MessageHandler> rh = 0;
MessageHandler* MessageHandler::instance()
{
    if(!rh.valid())
    {
        rh = new MessageHandler;
    }
    return rh.get();
}
*/

void MessageHandler::setRoot(osg::Group *root)
{
    _root = root;
    reset();
}

bool MessageHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    if( !_root.valid() )
        return false;

    if (ea.getHandled() == true)
    {
        return true;
    }

    bool out = false;
    switch(ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
    {
        out = handleFrame(ea, aa);
    }
    case osgGA::GUIEventAdapter::PUSH:
    {
        out = handlePush(ea, aa);
    }
        break;
    default :
        break;
    }

    return out;
}

bool MessageHandler::handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    handleNewScene();

    osgScene::NodeList tmp_AddNodes;
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeAddMutex);
        tmp_AddNodes.swap(_nodesAddToScene);
    }
    handleAddNode(tmp_AddNodes);

    osgScene::NodeList tmp_DeleteNodes;
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeDeleteMutex);
        tmp_DeleteNodes.swap(_nodesDeleteFromScene);
    }
    handleDeleteNode(tmp_DeleteNodes);

    handleCommandBuffer();

    handleResetCameraPosition(ea, aa);

    return false;
}

bool MessageHandler::addNodeToScene(osg::MatrixTransform *node)
{
    if(!_root.valid())
        return false;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeAddMutex);
    _nodesAddToScene.push_back(node);

    return true;
}

bool MessageHandler::addNodesToScene(const osgScene::NodeList &nv)
{
    if(!_root.valid())
        return false;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeAddMutex);

    /*
    for(NodeList::const_iterator itr = nv.begin(); itr != nv.end(); itr++)
    {
        _nodesAddToScene.push_back( itr->get() );
    }
    */
    std::copy(nv.begin(), nv.end(), std::back_inserter(_nodesAddToScene));

    return true;
}

bool MessageHandler::deleteNodeFromScene(osg::MatrixTransform *node)
{
    if(!_root.valid())
        return false;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeDeleteMutex);
    _nodesDeleteFromScene.push_back(node);

    return true;
}

bool MessageHandler::deleteNodesFromScene(const osgScene::NodeList &nv)
{
    if(!_root.valid())
        return false;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_nodeDeleteMutex);

    /*
    for(NodeList::const_iterator itr = nv.begin(); itr != nv.end(); itr++)
    {
        _nodesDeleteFromScene.push_back( itr->get() );
    }
    */
    std::copy(nv.begin(), nv.end(), std::back_inserter(_nodesDeleteFromScene));

    return true;
}

bool MessageHandler::handleAddNode(const osgScene::NodeList &nodes)
{
    osgScene::NodeList::const_iterator itr = nodes.begin();
    for(itr; itr != nodes.end(); itr++)
    {
        _root->addChild( itr->get() );
    }

    return true;
}

bool MessageHandler::handleDeleteNode(const osgScene::NodeList &nodes)
{
    osgScene::NodeList::const_iterator itr = nodes.begin();
    for(itr; itr != nodes.end(); itr++)
    {
        _root->removeChild( itr->get() );
    }

    return true;
}

void MessageHandler::newScene()
{
    ++_newScene;
}

void MessageHandler::resetCameraPosition()
{
    ++_resetCameraPosition;
}

bool MessageHandler::handleNewScene()
{
    int scene = _newScene;
    if( scene>0 )
    {
        _root->removeChildren(0, _root->getNumChildren());
        --_newScene;
    }
    return false;
}

bool MessageHandler::handleResetCameraPosition(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    int reset = _resetCameraPosition;
    if( reset>0 )
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if(viewer)
            viewer->home();
        --_resetCameraPosition;
    }

    return false;
}
