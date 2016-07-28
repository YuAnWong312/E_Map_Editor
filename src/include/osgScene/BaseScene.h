#ifndef BASESCENE_H
#define BASESCENE_H 1

#include <OpenThreads/Mutex>
#include <osg/observer_ptr>
#include <osgViewer/Viewer>
#include <osgGA/StandardManipulator>
#include <osgScene/Export.h>

class QWidget;

namespace osgScene
{

class OSGSCENE_EXPORT BaseScene : public osg::Referenced
{
public:
    BaseScene();
    virtual ~BaseScene(){}

    virtual osg::Camera* createCameraWithQWidget(int x, int y, int w, int h);

    virtual void init();
    virtual bool openFile(const std::string& path);
    virtual bool saveFile(const std::string& path);

    virtual void run();
    virtual void frame();
    virtual void setDone();
    bool done();

    void setMasterCamera(osg::Camera* camera);
    void setCameraTravelMask(unsigned int mask);
    void addEventHandler(osgGA::GUIEventHandler* eh);
    QWidget* getWidget();

    inline osgViewer::Viewer* getViewer() { return &_viewer; }
    inline osg::Group* getScene() { return _scene.get(); }

protected:
    osgViewer::Viewer                _viewer;        //viewer
    osg::observer_ptr<osg::Group>    _root;          //scene data root
    osg::observer_ptr<osg::Group>    _scene;         //add or delete node root

    OpenThreads::Mutex _globalMutex;
    bool _init;
};

}
/*
virtual void updateManipulator()
{
    if (_manipulator.valid())
    {
        _manipulator->setNode(_viewer.getSceneData());
    }
}
*/

#endif
