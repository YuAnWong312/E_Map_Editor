#ifndef PICKHANDLER_H
#define PICKHANDLER_H 1

#include <vector>

#include <OpenThreads/Mutex>
#include <osgGA/GUIEventHandler>

#include <osgScene/SceneUtils.h>

#include <MapFramework/Export.h>

namespace osgViewer
{
class Viewer;
}

class MAPGUI_EXPORT PickHandler : public osgGA::GUIEventHandler
{
public:
    PickHandler();

	virtual bool handle (const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	virtual osg::MatrixTransform* pick(osgViewer::Viewer* viewer, float x, float y);

	void setIntersectionMask(osg::Node::NodeMask mask);
	void reset();

protected:
    virtual ~PickHandler();
    virtual bool cheakNode(osg::MatrixTransform* mt);

protected:
    osgScene::SelectionList _pickSelections;
	osg::Node::NodeMask _intersectionMask;
};

#endif
