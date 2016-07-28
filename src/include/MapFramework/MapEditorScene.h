#ifndef MAPEDITORSCENE_H
#define MAPEDITORSCENE_H 1

#include <osgDragger/HudCamera>
#include <osgScene/BaseScene.h>
#include <osgScene/SceneUtils.h>

#include <MapFramework/Export.h>

namespace MapGUI
{
class MAPGUI_EXPORT MapEditorScene : public osgScene::BaseScene
{
public:
    MapEditorScene();
    virtual ~MapEditorScene();

    //reimplement
    virtual void init();
protected:
    osg::ref_ptr<osgDragger::HudCamera> _hudCamera;
};
}
#endif
