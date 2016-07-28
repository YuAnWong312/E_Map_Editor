
#include <MapFramework/Widget.h>
#include <MapFramework/MapEditorScene.h>
#include <MapFramework/MapShowScene.h>

namespace MapGUI
{

EditorWidget::EditorWidget(QWidget *parent, Qt::WindowFlags f) :
    RenderWidget(parent, f)
{
    osg::ref_ptr<MapEditorScene> scene = new MapEditorScene;
    scene->init();

    //_messageHandler = new MapGUIMessageHandler;
    _messageHandler = MapGUIMessageHandler::instance();
    scene->addEventHandler(_messageHandler.get());
    _messageHandler->setRoot(scene->getScene());

    setScene(scene.get());
}

ShowWidget::ShowWidget(QWidget *parent, Qt::WindowFlags f) :
    RenderWidget(parent, f)
{
    osg::ref_ptr<MapShowScene> scene = new MapShowScene;
    scene->init();

    _messageHandler = new MapGUIMessageHandler;
    scene->addEventHandler(_messageHandler.get());
    _messageHandler->setRoot(scene->getScene());

    setScene(scene.get());
}

}
