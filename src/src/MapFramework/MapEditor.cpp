
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
    _newFile = new QAction(QIcon("./images/new.png"), "&�½�", this);
    _newFile->setStatusTip("�½�");
    connect(_newFile, SIGNAL(triggered()), this, SLOT(newFile()));

    _openFile = new QAction(QIcon("./images/open.png"), "&��", this);
    _openFile->setStatusTip("��");
    connect(_openFile, SIGNAL(triggered()), this, SLOT(openFile()));

    _saveFile = new QAction(QIcon("./images/save.png"), "&����", this);
    _saveFile->setStatusTip("����");
    connect(_saveFile, SIGNAL(triggered()), this, SLOT(saveFile()));

    _sceneAct = new QAction(QIcon("./images/scene.png"), "&����", this);
    _sceneAct->setStatusTip("����ѡ��");
    connect(_sceneAct, SIGNAL(triggered()), this, SLOT(slotScene()));

    _transAct = new QAction(QIcon("./images/move.png"), "&�ƶ�ѡ��", this);
    _transAct->setStatusTip("�ƶ�ѡ��");
    connect(_transAct, SIGNAL(triggered()), this, SLOT(slotMove()));

    _rotateAct = new QAction(QIcon("./images/rotate.png"), "&��תѡ��", this);
    _rotateAct->setStatusTip("��תѡ��");
    connect(_rotateAct, SIGNAL(triggered()), this, SLOT(slotRotate()));

    _delteAct = new QAction(QIcon("./images/delete.png"), "&ɾ��ѡ��", this);
    _delteAct->setStatusTip("ɾ��ѡ��");
    connect(_delteAct, SIGNAL(triggered()), this, SLOT(delteNode()));

    _duplicateAct = new QAction(QIcon("./images/copy.png"), "&����ѡ��", this);
    _duplicateAct->setStatusTip("����ѡ��");
    connect(_duplicateAct, SIGNAL(triggered()), this, SLOT(duplicateNode()));
}

void MapEditor::createStatusBar()
{
    statusBar()->showMessage("���");
}

void MapEditor::createToolBar()
{
    _fileToolBar = addToolBar("�ļ�");
    _fileToolBar->setMovable(false);
    _fileToolBar->addAction(_newFile);
    _fileToolBar->addAction(_openFile);
    _fileToolBar->addAction(_saveFile);

    _editToolBar = addToolBar("�༭");
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
    _leftDock = new QDockWidget("�豸�б�",this);
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
        _objectRecordDock = new QDockWidget("������豸", this);
        _objectRecordDock->setFeatures(_objectRecordDock->features() & ~QDockWidget::DockWidgetClosable);
        _objectRecordDock->setObjectName("add");
        _objectRecordDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, _objectRecordDock);

        _objectRecordWidget = new ObjectsRecordListWidget(_objectRecordDock);
        _objectRecordDock->setWidget(_objectRecordWidget);
    }

    {
        _propertyDock= new QDockWidget("����",this);
        _propertyDock->setFeatures(_propertyDock->features() & ~QDockWidget::DockWidgetClosable);
        _propertyDock->setObjectName("property");
        _propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, _propertyDock);

        _propertyWidget = new PropertyEditor(_propertyDock);
        _propertyDock->setWidget(_propertyWidget);
    }

    /*
    {
        _databaseDock= new QDockWidget("���ݿ���Ϣ",this);
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
        int ret = QMessageBox::warning(this, "��ʾ", "�����ı䣬�Ƿ񱣴�", QMessageBox::Save | QMessageBox::Discard);

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
        int ret = QMessageBox::warning(this, "��ʾ", "�����ı䣬�Ƿ񱣴�", QMessageBox::Save | QMessageBox::Discard);

        if(QMessageBox::Save == ret)
            saveFile ();
    }

    QString fileName = QFileDialog::getOpenFileName(this, "�����ļ�", "./SceneMap", "");
    //*.osgb *.osg *.osgt *.ive *.osgx

    if(fileName.isEmpty())
        return;

    std::string path(fileName.toStdString());

    bool output = _widget->getMessageHandler()->openFile(path);
    if (output)
    {
        _sceneChange = true;
        QMessageBox::warning (this, "��ʾ", "���سɹ�");
    }
    else
    {
        _sceneChange = false;
        QMessageBox::warning (this, "��ʾ", "����ʧ��");
    }
    _needResetViewPoint = false;
}

bool MapEditor::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "�����ļ�", "./SceneMap/untitled.osgb", "osgb (*.osgb)");

    if(fileName.isEmpty())
        return false;

    std::string path(fileName.toStdString());

    bool output = _widget->getScene()->saveFile(path);
    if (output)
    {
        QMessageBox::warning (this, "��ʾ", "����ɹ�");
    }
    else
    {
        QMessageBox::warning (this, "��ʾ", "����ʧ��");
    }

    _sceneChange = false;
    return true;
}

void MapEditor::slotScene()
{
    //������ק��������
    _widget->getMessageHandler()->addCommand(3);
}

void MapEditor::slotMove()
{
    //��ʾƽ����ק��
    _widget->getMessageHandler()->addCommand(1);
}

void MapEditor::slotRotate()
{
    //��ʾ��ת��ק��
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
    QMessageBox::warning(this, "��ʾ", QString("����:%1").arg(size));
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
