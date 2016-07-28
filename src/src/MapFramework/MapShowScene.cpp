
#include <osgDB/ReadFile>
#include <MapFramework/MapShowScene.h>
#include <MapFramework/Define.h>
#include <MapFramework/SceneManipulator.h>
#include <MapFramework/StateCollect.h>
#include <MapFramework/ViewController.h>

using namespace MapGUI;
bool Handler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
    if(ea.getKey() == 'q')
    {
        //stateset->removeAttribute(program.get());
        //stateset->setAttributeAndModes(program.get(), osg::StateAttribute::OFF);
        for (osgScene::SelectionList::iterator itr = _sl.begin(); itr != _sl.end(); itr++)
        {
            if ( (*itr).valid())
            {
                (*itr)->setStateSet(stateset.get());
            }
        }
    }

    if (ea.getKey() == 'w')
    {
        //stateset->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
        for (osgScene::SelectionList::iterator itr = _sl.begin(); itr != _sl.end(); itr++)
        {
            if ( (*itr).valid())
            {
                (*itr)->setStateSet(0);
            }
        }
    }

    return false;
}

void ColorCallback::operator ()(osg::Uniform* uniform, osg::NodeVisitor* nv)
{
    if (!uniform && uniform->getName() != "red")
    {
        return;
    }

    float red;
    uniform->get(red);

    if(_inc == true)
    {
        if(red<1.0f)
            red+=0.01f;
        else
            _inc = false;
    }
    else
    {
        if(red>0.5f)
            red-=0.01f;
        else
            _inc = true;
    }
    uniform->set(red);

}

MapShowScene::MapShowScene()
{

}

void MapShowScene::init()
{
    if (_init)
    {
        return;
    }

    //osg::ref_ptr<ReadAndShareCallback> sharer = new ReadAndShareCallback;
    //osgDB::Registry::instance()->setReadFileCallback( sharer.get() );

    osg::Camera* camera = createCameraWithQWidget(0, 0, OSG_WIDGET_WIDTH, OSG_WIDGET_HEIGHT);
    _viewer.setCamera( camera );

    _root = new osg::Group;
    ViewController _viewController(_root.get(), 0, 0);

    _scene = new osg::MatrixTransform;
    _scene->setNodeMask(SCENE_NODEMASK);
    _root->addChild(_scene.get());

    _handler = new Handler;
    _blankStateset = setupStateSet();

    _viewer.setSceneData(_root.get());
    _viewer.addEventHandler(new ShowMessageHandler);
    _viewer.addEventHandler(_handler.get());
    _viewer.setKeyEventSetsDone(0);

    _hudCamera = new osgDragger::HudCamera(camera, 60, 60);


    osg::ref_ptr<CustomerSceneManipulator> manipulator =
            new CustomerSceneManipulator(osgGA::OrbitManipulator::DEFAULT_SETTINGS | osgGA::OrbitManipulator::COMPUTE_HOME_USING_BBOX /*| osgGA::OrbitManipulator::SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT*/);

    manipulator->setCamera(_viewer.getCamera());

    _viewer.setCameraManipulator(manipulator.get());
    _viewer.home();

    _init = true;
}

/*
        for (SelectionList::iterator itr = _sl.begin(); itr != _sl.end(); itr++)
        {
            if ( (*itr).valid())
            {
                (*itr)->setStateSet(_blankStateset.get());
            }
        }
        */


//_handler->_sl = _sl;

osg::StateSet* MapShowScene::setupStateSet()
{
    osg::StateSet* stateset = new osg::StateSet;

    osg::ref_ptr<osg::Shader> vertexShader =
            osgDB::readShaderFile(osg::Shader::VERTEX, "./images/vertex.vert");

    osg::ref_ptr<osg::Shader> FragShader =
            osgDB::readShaderFile(osg::Shader::FRAGMENT, "./images/frag.frag");

    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(vertexShader.get());
    program->addShader(FragShader.get());

    osg::Uniform* mainColor = new osg::Uniform("red", 1.0f);
    mainColor->setUpdateCallback(new ColorCallback);
    stateset->addUniform(mainColor);
    stateset->setAttributeAndModes(program.get());

    if (_handler.valid())
    {
        _handler->program = program.get();
        _handler->stateset = stateset;
    }
    return stateset;
}
