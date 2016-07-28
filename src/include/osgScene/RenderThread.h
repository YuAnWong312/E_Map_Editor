
#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H 1

#include <osg/observer_ptr>
#include <QThread>
#include <osgScene/BaseScene.h>
#include <osgScene/Export.h>

namespace osgScene
{

class OSGSCENE_EXPORT RenderThread : public QThread
{
public:
    RenderThread(QObject * parent = 0);
    virtual ~RenderThread();

    void setScene(BaseScene* scene);

protected:
    virtual void run();

protected:
    osg::observer_ptr<BaseScene> _scene;
};

}

#endif
