
#ifndef WIDGET_H
#define WIDGET_H 1

#include <osgScene/RenderWidget.h>
#include <MapFramework/MapGUIMessageHandler.h>

namespace MapGUI
{
class MAPGUI_EXPORT EditorWidget : public osgScene::RenderWidget
{
    Q_OBJECT
public:
    EditorWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

    inline MapGUIMessageHandler* getMessageHandler() { return _messageHandler.get(); }

protected:
    osg::observer_ptr<MapGUIMessageHandler> _messageHandler;

};

class MAPGUI_EXPORT ShowWidget : public osgScene::RenderWidget
{
    Q_OBJECT
public:
    ShowWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

    inline MapGUIMessageHandler* getMessageHandler() { return _messageHandler.get(); }

protected:
    osg::observer_ptr<MapGUIMessageHandler> _messageHandler;
};
}
#endif
