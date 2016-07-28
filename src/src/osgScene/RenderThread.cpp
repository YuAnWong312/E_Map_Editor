
#include <osgScene/RenderThread.h>

namespace osgScene
{
RenderThread::RenderThread(QObject *parent) :
    QThread(parent), _scene(0)
{
    qDebug( "QRenderThread Initial threadID = %d", QThread::currentThreadId() );
}

RenderThread::~RenderThread()
{
    if(_scene.valid())
    {
        qDebug( "QRenderThread Destroy threadID = %d", QThread::currentThreadId() );

        // consider it need
        //_viewer->setDone(true);

        _scene = 0;
        wait();
    }
}

void RenderThread::setScene(BaseScene* scene)
{
    if(_scene == scene)
        return;

    _scene = scene;
}

void RenderThread::run()
{
    osg::ref_ptr<BaseScene> scene;

    qDebug( "QRenderThread run threadID = %d", QThread::currentThreadId() );

    while( _scene.lock(scene) && !scene->done() )
    {
        scene->frame();
    }
}
}
