
#include <MapFramework/MapEditorScene.h>
#include <MapFramework/SceneManipulator.h>
#include <MapFramework/ViewController.h>
#include <MapFramework/Define.h>
#include <MapFramework/PickHandler.h>
#include <MapFramework/CollectVisitor.h>
#include <MapFramework/MapGUIMessageHandler.h>
#include <MapFramework/MapDragger.h>

#include <osgScene/ReadAndShareCallback.h>

#include <osg/ShapeDrawable>
#include <osg/ValueObject>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/ViewerEventHandlers>

#include <osgDragger/ShowBound>
#include <osgDragger/ClearDepth>

namespace
{
osg::Camera* createHUD(osg::GraphicsContext* gc)
{
    if (gc)
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setClearMask(GL_DEPTH_BUFFER_BIT);
        camera->setRenderOrder(osg::Camera::POST_RENDER,10);
        camera->setAllowEventFocus(true);
        camera->setGraphicsContext(gc);
        const osg::GraphicsContext::Traits* traits = gc->getTraits();
        camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);

        return camera.release();
    }
    return 0;
}
}

using namespace MapGUI;
MapEditorScene::MapEditorScene()
{
}

MapEditorScene::~MapEditorScene()
{
}

void MapEditorScene::init()
{
    if (_init)
    {
        return;
    }

    osg::ref_ptr<osgScene::ReadAndShareCallback> sharer = new osgScene::ReadAndShareCallback;
    osgDB::Registry::instance()->setReadFileCallback( sharer.get() );

    _viewer.setKeyEventSetsDone(0);
    osg::Camera* camera = createCameraWithQWidget(0, 0, OSG_WIDGET_WIDTH, OSG_WIDGET_HEIGHT);
    _viewer.setCamera(camera);
    //master camera

    _root = new osg::Group;
    _viewer.setSceneData(_root.get()); //scene root

    _scene = new osg::MatrixTransform;
    _scene->setNodeMask( SCENE_NODEMASK );
    _scene->setInitialBound(osg::BoundingSphere(osg::Vec3(0.0f, 0.0f, 0.0f), 50.0f));
    _root->addChild(_scene.get());

    osg::ref_ptr<PickHandler> pickHandler = new PickHandler;
    pickHandler->setIntersectionMask(PICK_HANDLEMASK);
    _viewer.addEventHandler(pickHandler.get());

    osgDragger::ShowBound::Instence()->setNodeMask(SHOWBOUND_NODEMASK);
    //_root->addChild(osgDragger::ShowBound::Instence()->getRoot());

    /////////////////////////////////////////////////
    ////////////////////////////////////////////////
    osg::ref_ptr<osg::Camera> hudCamera = 0;

    //osgViewer::Viewer::Windows windows;
    //_viewer->getWindows(windows);
    //if (windows.size() != 0)
    //{
    //	hudCamera = createHUD(windows[0]);
    //	osg::ref_ptr<osg::Group> hudRoot = new osg::Group;
    //	hudCamera->addChild(hudRoot.get());
    //	hudRoot->addChild(CustomerDraggerManager::Instence()->getRoot());
    //	hudRoot->addChild(osgDragger::ShowBound::Instence()->getRoot());
    //	_viewer->addSlave(hudCamera.get(), false);
    //	_hudCamera = new osgDragger::HudCamera(hudCamera.get(), 60, 60);
    //}
    //else
    {
        CustomerDraggerManager::Instence()->getRoot()->getOrCreateStateSet()->setAttributeAndModes(new osgDragger::ClearDepth);
        CustomerDraggerManager::Instence()->getRoot()->getOrCreateStateSet()->setRenderBinDetails(10, "RenderBin");
        _root->addChild(CustomerDraggerManager::Instence()->getRoot());
        _root->addChild(osgDragger::ShowBound::Instence()->getRoot());
        _hudCamera = new osgDragger::HudCamera(camera, 60, 60);
    }

    osg::ref_ptr<CustomerMoveDragger> moveDragger = new CustomerMoveDragger;
    moveDragger->setName("move");
    moveDragger->setIntersectionMask(DRAG_HANDLEMASK);
    moveDragger->setIntersectionCamera(hudCamera.get());
    moveDragger->setupDefaultGeometry();
    moveDragger->setHandleEvents(true);
    CustomerDraggerManager::Instence()->addDragger(moveDragger, true);
    //CustomerDraggerManager::Instence()->setIntersectionMask(0x20000000);

    QObject::connect(moveDragger, SIGNAL(signalMoveFinish()), CustomerDraggerManager::Instence(), SIGNAL(signalMoveFinish()));

    osg::ref_ptr<CustomerRotateDragger> rotateDragger = new CustomerRotateDragger;
    rotateDragger->setName("rotate");
    rotateDragger->setIntersectionMask(DRAG_HANDLEMASK);
    rotateDragger->setIntersectionCamera(hudCamera.get());
    rotateDragger->setupDefaultGeometry();
    rotateDragger->setHandleEvents(true);
    CustomerDraggerManager::Instence()->addDragger(rotateDragger, false);
    //CustomerDraggerManager::Instence()->setIntersectionMask(0x20000000);

    QObject::connect(rotateDragger, SIGNAL(signalMoveFinish()), CustomerDraggerManager::Instence(), SIGNAL(signalMoveFinish()));

    //_hudDraggerCamera = new osgDragger::HudDraggerCamera(camera, CustomerDraggerManager::Instence()->getRoot());
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////

    //ViewController _viewController(_root.get(), 410+VIEWCONTROLLER_CAMERA_SIZE, 447+VIEWCONTROLLER_CAMERA_SIZE);
    ViewController _viewController(_root.get(), 0, 0);

    osg::ref_ptr<CustomerSceneManipulator> manipulator =
            new CustomerSceneManipulator(osgGA::OrbitManipulator::DEFAULT_SETTINGS | osgGA::OrbitManipulator::COMPUTE_HOME_USING_BBOX);

    manipulator->setCamera(_viewer.getCamera());
    _viewer.setCameraManipulator(manipulator.get(), false);
    manipulator->setNode(_scene.get());
    _init = true;
}
