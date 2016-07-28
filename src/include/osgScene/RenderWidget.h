#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H 1

#include <osgScene/BaseScene.h>
#include <osgScene/RenderThread.h>

#include <QWidget>
#include <QTimer>

class QVBoxLayout;

namespace osgScene
{
//#define USE_THREAD
class OSGSCENE_EXPORT RenderWidget : public QWidget
{
    Q_OBJECT
public:
    RenderWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~RenderWidget();

    void setScene(BaseScene* scene);
    BaseScene* getScene(){ return _scene.get(); }

    void run();
    //virtual QSize sizeHint () const;

protected:
#ifndef USE_THREAD
    virtual void paintEvent(QPaintEvent* event);
#endif

    virtual void init();

protected:
    osg::ref_ptr<BaseScene> _scene;
    osgScene::RenderThread _renderThread;
    QTimer _timer;
    QVBoxLayout* _layout;

};
}
#endif
