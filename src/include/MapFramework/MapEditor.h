#ifndef MAPEDITOR_H
#define MAPEDITOR_H 1

#include <QMainWindow>
#include <MapFramework/Widget.h>

namespace MapGUI
{
class PropertyEditor;
class ObjectsRecordListWidget;
class ToolList;
class DatabaseWidget;
}

class QAction;
class QMdiArea;

namespace MapGUI
{

class MAPGUI_EXPORT MapEditor : public QMainWindow
{
	Q_OBJECT
public:
	MapEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~MapEditor();
	
protected:
	virtual void createAction();
	virtual void createToolBar();
	virtual void createStatusBar();
    virtual void createCentralWidget();

    void createLeftDockWidget();
	void createRightDockWidget();

protected slots:
	void newFile();
	void openFile();
	bool saveFile();

	void slotScene();
	void slotMove();
	void slotRotate();

	void delteNode();
	void duplicateNode();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

protected:

    QMdiArea* _mdiArea;
	QToolBar* _fileToolBar;
	QToolBar* _editToolBar;

	QAction* _newFile;
	QAction* _openFile;
	QAction* _saveFile;

	QAction* _sceneAct;
	QAction* _transAct;
	QAction* _rotateAct;

	QAction* _delteAct;
	QAction* _duplicateAct;

    QDockWidget* _leftDock;
    QDockWidget* _objectRecordDock;
    QDockWidget* _propertyDock;
    QDockWidget* _databaseDock;

    ToolList* _drag;
    ObjectsRecordListWidget* _objectRecordWidget;
    PropertyEditor* _propertyWidget;
    DatabaseWidget* _databaseWidget;
    //osg::observer_ptr<MapGUI::MapEditorScene> _currentScene;
    EditorWidget* _widget;
    bool _sceneChange;
    bool _needResetViewPoint;
};

}
#endif // MAPEDITOR_H
