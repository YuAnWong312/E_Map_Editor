#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H 1

#include <vector>

#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include <OpenThreads/Atomic>

#include <osg/observer_ptr>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventHandler>

#include <osgScene/SceneUtils.h>
#include <osgScene/Export.h>

namespace osgScene
{

class OSGSCENE_EXPORT MessageHandler : public osgGA::GUIEventHandler
{
public:
    MessageHandler();

    virtual void setRoot(osg::Group* root);
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

    virtual bool addNodeToScene(osg::MatrixTransform* node);
    virtual bool addNodesToScene(const osgScene::NodeList& nv);

    virtual bool deleteNodeFromScene(osg::MatrixTransform* node);
    virtual bool deleteNodesFromScene(const osgScene::NodeList& nv);

    virtual void newScene();
    virtual void reset(){}

    virtual void resetCameraPosition();

protected:
    virtual bool handleFrame(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual bool handlePush(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa){ return false; }

    virtual bool handleCommandBuffer() { return false; }

    virtual bool handleAddNode(const osgScene::NodeList& nodes);
    virtual bool handleDeleteNode(const osgScene::NodeList& nodes);
    virtual bool handleNewScene();

    virtual bool handleResetCameraPosition(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
protected:

    osg::observer_ptr<osg::Group> _root;

    OpenThreads::Mutex _nodeAddMutex;
    OpenThreads::Mutex _nodeDeleteMutex;
    OpenThreads::Mutex _commandBufferMutex;

    mutable OpenThreads::Atomic _newScene;

    osgScene::NodeList _nodesAddToScene;
    osgScene::NodeList _nodesDeleteFromScene;

    mutable OpenThreads::Atomic _resetCameraPosition;
};

}
#endif
