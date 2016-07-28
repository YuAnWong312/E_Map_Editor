
#include <MapFramework/MapEditor.h>

#include <MapFramework/ToolList.h>
#include <MapFramework/ObjectsRecordListWidget.h>
#include <MapFramework/PropertyEditor.h>
#include <MapFramework/DatabaseWidget.h>
#include <MapFramework/MapDragger.h>

#include <QAction>
#include <QMdiArea>

using namespace MapGUI;

MapEditor::MapEditor(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags), _sceneChange(false), _needResetViewPoint(true)
{
    createAction();
    createToolBar();
    createStatusBar();
    createLeftDockWidget();
    createRightDockWidget();
    createCentralWidget();

    this->setAcceptDrops(true);

    QObject::connect( _widget->getMessageHandler(), SIGNAL(signalAddListWidgetItem(Items&)), _objectRecordWidget, SLOT(addItemsToMe(Items&)) );

    QObject::connect( _widget->getMessageHandler(), SIGNAL(signalDeleteListWidgetItem(Items&)), _objectRecordWidget, SLOT(deleteItems(Items&)) );

    QObject::connect( _widget->getMessageHandler(), SIGNAL(signalReset()), _objectRecordWidget, SLOT(reset()) );

    QObject::connect( _widget->getMessageHandler(), SIGNAL(signalReset()), _propertyWidget, SLOT(reset()) );

    QObject::connect( _widget->getMessageHandler(), SIGNAL(signalPositionInfo(QString)), _propertyWidget, SLOT(receivePropertyChange(QString)) );

    //QObject::connect( _widget->getMessageHandler(), SIGNAL(signalAddDatabaseItem(void*)), _databaseWidget, SLOT(addRow(void*)) );

    //QObject::connect( _widget->getMessageHandler(), SIGNAL(signalDeleteDatabaseItem(void*)), _databaseWidget, SLOT(removeRow(void*)) );

    QObject::connect( _propertyWidget, SIGNAL(emitPropertyChange(QString)), _widget->getMessageHandler(), SLOT(slotPositionInfoChange(QString)) );

    QObject::connect( _objectRecordWidget, SIGNAL(signalSelectionChange(QListWidgetItem*)), _widget->getMessageHandler(), SLOT(slotSelectionItemChange(QListWidgetItem*)) );

    QObject::connect( CustomerDraggerManager::Instence(), SIGNAL(signalMoveFinish()), _widget->getMessageHandler(), SLOT(slotMoveFinish()) );
}

MapEditor::~MapEditor()
{

}

void MapEditor::createAction()
{
    _newFile = new QAction(QIcon("./images/new.png"), "&新建", this);
    _newFile->setStatusTip("新建");
    connect(_newFile, SIGNAL(triggered()), this, SLOT(newFile()));

    _openFile = new QAction(QIcon("./images/open.png"), "&打开", this);
    _openFile->setStatusTip("打开");
    connect(_openFile, SIGNAL(triggered()), this, SLOT(openFile()));

    _saveFile = new QAction(QIcon("./images/save.png"), "&保存", this);
    _saveFile->setStatusTip("保存");
    connect(_saveFile, SIGNAL(triggered()), this, SLOT(saveFile()));

    _sceneAct = new QAction(QIcon("./images/scene.png"), "&场景", this);
    _sceneAct->setStatusTip("场景选择");
    connect(_sceneAct, SIGNAL(triggered()), this, SLOT(slotScene()));

    _transAct = new QAction(QIcon("./images/move.png"), "&移动选择", this);
    _transAct->setStatusTip("移动选择");
    connect(_transAct, SIGNAL(triggered()), this, SLOT(slotMove()));

    _rotateAct = new QAction(QIcon("./images/rotate.png"), "&旋转选择", this);
    _rotateAct->setStatusTip("旋转选择");
    connect(_rotateAct, SIGNAL(triggered()), this, SLOT(slotRotate()));

    _delteAct = new QAction(QIcon("./images/delete.png"), "&删除选择", this);
    _delteAct->setStatusTip("删除选择");
    connect(_delteAct, SIGNAL(triggered()), this, SLOT(delteNode()));

    _duplicateAct = new QAction(QIcon("./images/copy.png"), "&复制选择", this);
    _duplicateAct->setStatusTip("复制选择");
    connect(_duplicateAct, SIGNAL(triggered()), this, SLOT(duplicateNode()));
}

void MapEditor::createStatusBar()
{
    statusBar()->showMessage("完毕");
}

void MapEditor::createToolBar()
{
    _fileToolBar = addToolBar("文件");
    _fileToolBar->setMovable(false);
    _fileToolBar->addAction(_newFile);
    _fileToolBar->addAction(_openFile);
    _fileToolBar->addAction(_saveFile);

    _editToolBar = addToolBar("编辑");
    _editToolBar->setMovable(false);
    _editToolBar->addAction(_sceneAct);
    _editToolBar->addAction(_transAct);
    _editToolBar->addAction(_rotateAct);
    _editToolBar->addAction(_delteAct);
    _editToolBar->addAction(_duplicateAct);
}

void MapEditor::createCentralWidget()
{
    //_currentScene = new MapEditorScene;
    //_currentScene->init();
    //_currentScene->setCameraTravelMask(0xffffffff);
    //setCentralWidget(_sceneManager->getWidget());
    _widget = new EditorWidget;

    _mdiArea = new QMdiArea(this);
    _mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(_mdiArea);

    //RenderWidget* widget = new RenderWidget(_mdiArea);
    //widget->setScene(_currentScene.get());
    QMdiSubWindow* subWindow = new QMdiSubWindow;
    subWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    subWindow->setWidget(_widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    _mdiArea->addSubWindow(subWindow);
    _widget->showMaximized();
}

void MapEditor::createLeftDockWidget()
{
    _leftDock = new QDockWidget("设备列表",this);
    _leftDock->setFeatures(_leftDock->features() & ~QDockWidget::DockWidgetClosable);
    _leftDock->setObjectName("Drag");
    _leftDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea , _leftDock);

    _drag = new ToolList(_leftDock);
    _leftDock->setWidget(_drag);
}

void MapEditor::createRightDockWidget()
{
    {
        _objectRecordDock = new QDockWidget("已添加设备", this);
        _objectRecordDock->setFeatures(_objectRecordDock->features() & ~QDockWidget::DockWidgetClosable);
        _objectRecordDock->setObjectName("add");
        _objectRecordDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, _objectRecordDock);

        _objectRecordWidget = new ObjectsRecordListWidget(_objectRecordDock);
        _objectRecordDock->setWidget(_objectRecordWidget);
    }

    {
        _propertyDock= new QDockWidget("属性",this);
        _propertyDock->setFeatures(_propertyDock->features() & ~QDockWidget::DockWidgetClosable);
        _propertyDock->setObjectName("property");
        _propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, _propertyDock);

        _propertyWidget = new PropertyEditor(_propertyDock);
        _propertyDock->setWidget(_propertyWidget);
    }

    /*
    {
        _databaseDock= new QDockWidget("数据库信息",this);
        _databaseDock->setFeatures(_databaseDock->features() & ~QDockWidget::DockWidgetClosable);
        _databaseDock->setObjectName("database");
        _databaseDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, _databaseDock);

        _databaseWidget = new DatabaseWidget(_databaseDock);
        _databaseDock->setWidget(_propertyWidget);
    }
    */
}

void MapEditor::newFile()
{
    if (_sceneChange)
    {
        int ret = QMessageBox::warning(this, "提示", "场景改变，是否保存", QMessageBox::Save | QMessageBox::Discard);

        if(QMessageBox::Save == ret)
            saveFile();
    }

    _widget->getMessageHandler()->newScene();
    _sceneChange = false;
    _needResetViewPoint = true;
}

void MapEditor::openFile()
{
    if (_sceneChange)
    {
        int ret = QMessageBox::warning(this, "提示", "场景改变，是否保存", QMessageBox::Save | QMessageBox::Discard);

        if(QMessageBox::Save == ret)
            saveFile ();
    }

    QString fileName = QFileDialog::getOpenFileName(this, "加载文件", "./SceneMap", "");
    //*.osgb *.osg *.osgt *.ive *.osgx

    if(fileName.isEmpty())
        return;

    std::string path(fileName.toStdString());

    bool output = _widget->getMessageHandler()->openFile(path);
    if (output)
    {
        _sceneChange = true;
        QMessageBox::warning (this, "提示", "加载成功");
    }
    else
    {
        _sceneChange = false;
        QMessageBox::warning (this, "提示", "加载失败");
    }
    _needResetViewPoint = false;
}

bool MapEditor::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "./SceneMap/untitled.osgb", "osgb (*.osgb)");

    if(fileName.isEmpty())
        return false;

    std::string path(fileName.toStdString());

    bool output = _widget->getScene()->saveFile(path);
    if (output)
    {
        QMessageBox::warning (this, "提示", "保存成功");
    }
    else
    {
        QMessageBox::warning (this, "提示", "保存失败");
    }

    _sceneChange = false;
    return true;
}

void MapEditor::slotScene()
{
    //所有拖拽器都隐藏
    _widget->getMessageHandler()->addCommand(3);
}

void MapEditor::slotMove()
{
    //显示平移拖拽器
    _widget->getMessageHandler()->addCommand(1);
}

void MapEditor::slotRotate()
{
    //显示旋转拖拽器
    _widget->getMessageHandler()->addCommand(2);
}

void MapEditor::delteNode()
{
    bool deleteNode = true;
    _widget->getMessageHandler()->deleteSelectionNodes();

    if (deleteNode)
    {
        _sceneChange = true;
    }
}

void MapEditor::duplicateNode()
{
    int size = _widget->getMessageHandler()->duplicateNode();
    QMessageBox::warning(this, "提示", QString("复制:%1").arg(size));
    return;
}

void MapEditor::dragEnterEvent( QDragEnterEvent *event )
{
    event->setDropAction(Qt::MoveAction);
    event->acceptProposedAction();
}

void MapEditor::dropEvent( QDropEvent *event )
{
    MapGUI::DragList *source = qobject_cast<MapGUI::DragList*>(event->source());
    if ( source && event->mimeData()->hasText() )
    {
        event->setDropAction(Qt::MoveAction);
        event->acceptProposedAction();

        MapGUI::ItemInfo item;
        QString infostr = event->mimeData()->text();
        item.parseString(infostr);

        std::string modelPath = item._modelPath.toStdString();
        std::string name  = item._name.toStdString();
        int type = item._type;
        unsigned int mask = item._mask;

        osg::MatrixTransform* mt = _widget->getMessageHandler()->addNode(modelPath, name, type, mask, item._name);

        if (mt)
        {
            if(_needResetViewPoint)
            {
                _widget->getMessageHandler()->resetCameraPosition();
                _needResetViewPoint = false;
            }
            //_currentScene->getMessageHandler()->addNodeItemPair(mt, item._name);

            //add new object to database config
            //this is need code

            _sceneChange = true;
        }
    }
}
