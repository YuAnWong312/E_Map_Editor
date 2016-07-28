#include <osgScene/BaseScene.h>

#include <OpenThreads/ScopedLock>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgQt/GraphicsWindowQt>

#include <QCoreApplication>
#include <QResizeEvent>

namespace osgScene
{
BaseScene::BaseScene() :
    osg::Referenced(true), _init(false)
{ 
    osg::setNotifyLevel(osg::FATAL);
    osgDB::Registry::instance()->setBuildKdTreesHint(osgDB::Options::BUILD_KDTREES);
}

osg::Camera* BaseScene::createCameraWithQWidget( int x, int y, int w, int h )
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(ds);
    traits->windowDecoration = false;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    osg::ref_ptr<osgQt::GraphicsWindowQt> graphic = new osgQt::GraphicsWindowQt(traits.get());
    graphic->getGraphWidget()->setAcceptDrops(false);
    graphic->getGraphWidget()->setMinimumSize(10, 10);
    camera->setGraphicsContext(graphic.get());
    camera->setClearColor( osg::Vec4(0.39, 0.39, 0.39, 1.0) );
    GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
    camera->setDrawBuffer(buffer);
    camera->setReadBuffer(buffer);
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(
                30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 1000.0f );

    return camera.release();
}

void BaseScene::init()
{
    osg::Camera* camera = createCameraWithQWidget(0, 0, 100, 100);
    _viewer.setCamera(camera);

    osg::ref_ptr<osg::Group> root = new osg::Group;
    _root = root;
    _viewer.setSceneData(root.get());
    _viewer.setKeyEventSetsDone(0);

    osg::ref_ptr<osg::Group> scene = new osg::Group;
    _scene = scene;
    scene->setInitialBound(osg::BoundingSphere(osg::Vec3(0.0f, 0.0f, 0.0f), 50.0f));
    _root->addChild(scene.get());

    _init = true;
}

bool BaseScene::openFile(const std::string &path)
{
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(path);
    if(_scene.valid())
    {
        _scene->addChild(node.get());
        return true;
    }
    return false;
}

bool BaseScene::saveFile( const std::string& path )
{
    if (!_init)
    {
        init();
    }

    //osgDB::ReaderWriter* rw = osgDB::Registry::instance ()->getReaderWriterForExtension ("osgb");
    //osgDB::ReaderWriter::WriteResult success = rw->writeNode(*_scene, path);
    osg::ref_ptr<osgDB::Options> opt = new osgDB::Options;
    if (osgDB::getLowerCaseFileExtension(path) == "osgb")
    {
        opt->setOptionString("WriteImageHint=IncludeData");
    }

    bool success = false;
    osg::ref_ptr<osg::Group> node = 0;
    if(_scene.lock(node))
    {
        success = osgDB::writeNodeFile(*node, path, opt.get());
    }

    if(success)
    {
        return true;
    }

    return false;
}

void BaseScene::run()
{
    _viewer.run();
}

void BaseScene::frame()
{
    static bool resize = true;
    if(resize)
    {
        QWidget* widget = getWidget();
        if(widget)
        {
            QSize size = widget->size();
            //widget->resize(size);
            QCoreApplication::postEvent(widget, new QResizeEvent(size, size));
            resize = false;
        }
    }

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_globalMutex);
    _viewer.frame();
}

void BaseScene::setDone()
{
    _viewer.setDone(true);
}

bool BaseScene::done()
{
    return _viewer.done();
}

void BaseScene::setMasterCamera(osg::Camera *camera)
{
    _viewer.setCamera(camera);
}

void BaseScene::setCameraTravelMask( unsigned int mask )
{
    _viewer.getCamera()->setCullMask(mask);
}

void BaseScene::addEventHandler(osgGA::GUIEventHandler *eh)
{
    _viewer.addEventHandler(eh);
}

QWidget* BaseScene::getWidget()
{
    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( _viewer.getCamera()->getGraphicsContext() );

    if (gw)
    {
        return gw->getGLWidget();
    }
    else
        return 0;
}

}
