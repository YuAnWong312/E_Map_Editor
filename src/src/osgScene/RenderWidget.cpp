
#include <osgScene/RenderWidget.h>
#include <QVBoxLayout>

namespace osgScene
{

RenderWidget::RenderWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f), _scene(0)
{
    _timer.setInterval(10);

    _layout = new QVBoxLayout;
    //_layout->setContentsMargins(0,0,0,0);
    _layout->setSpacing(0);
    setLayout(_layout);
}

RenderWidget::~RenderWidget()
{
    if(_scene.valid())
    {
        _scene->setDone();
    }
}

void RenderWidget::setScene(BaseScene *scene)
{
    if(_scene.valid())
    {
        QWidget* child = _scene->getWidget();
        if(child)
        {
            _timer.stop();
            _scene->setDone();
            _layout->removeWidget(child);
            this->disconnect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
            if(_renderThread.isRunning())
            {
                _scene->setDone();
                _renderThread.wait();
            }
        }
    }

    _scene = scene;
    init();
}

void RenderWidget::run()
{
    _renderThread.start();
    _timer.start();
}

/*
QSize RenderWidget::sizeHint() const
{
    return QSize(200, 200);
}
*/

#ifndef USE_THREAD
void RenderWidget::paintEvent(QPaintEvent* event)
{
    if(_scene.valid())
        _scene->frame();
}
#endif

/*
void RenderWidget::closeEvent(QCloseEvent* event)
{
    event->accept();
    //this->deleteLater();
}
*/

void RenderWidget::init()
{
    QWidget* child = _scene->getWidget();
    if(child)
    {
        //_layout->addWidget(child);
        _layout->insertWidget(0, child, 1);
    }

#ifndef USE_THREAD
    connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start();
#else
    _renderThread.setScene(_scene.get());
    _renderThread.start();
#endif

}

}
