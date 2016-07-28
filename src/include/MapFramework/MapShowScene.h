
#ifndef MAPSHOWSCENE_H
#define MAPSHOWSCENE_H 1

#include <osg/Uniform>
#include <osgDragger/HudCamera>
#include <osgScene/SceneUtils.h>
#include <osgScene/BaseScene.h>

#include <MapFramework/Export.h>

namespace MapGUI
{
class MapShowScene;
class MAPGUI_EXPORT Handler : public osgGA::GUIEventHandler
{
public:
    virtual bool handle (const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
    friend class MapShowScene;
protected:
    osg::ref_ptr<osg::Program> program;
    osg::ref_ptr<osg::StateSet> stateset;
    osgScene::SelectionList _sl;
};

class MAPGUI_EXPORT ColorCallback : public osg::Uniform::Callback
{
public:
    ColorCallback():_inc(false){}
    virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* nv);
protected:
    bool _inc;
};

class MAPGUI_EXPORT MapShowScene : public osgScene::BaseScene
{
public:
    MapShowScene();
    virtual void init();
    osg::StateSet* setupStateSet();

protected:
    osg::ref_ptr<osg::StateSet> _blankStateset;
    osg::ref_ptr<Handler> _handler;

    osg::ref_ptr<osgDragger::HudCamera> _hudCamera;
};
}
#endif
