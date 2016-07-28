
#ifndef COLLECTVISITOR_H
#define COLLECTVISITOR_H 1

#include <osg/NodeVisitor>
#include <osgScene/SceneUtils.h>

#include <MapFramework/Export.h>

class MAPGUI_EXPORT CollectVisitor : public osg::NodeVisitor
{
public:
	CollectVisitor();
	virtual ~CollectVisitor();

	virtual void apply(osg::Node& node);

    void getSelections(osgScene::NodeList& nl, bool clearAfterCopy = true);

protected:
    osgScene::NodeList _nl;

};

#endif
