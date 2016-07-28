
#include <MapFramework/MapShow.h>
#include <MapFramework/TreeWidget.h>
#include <QGridLayout>
#include <MapFramework/Define.h>

using namespace MapGUI;

MapShow::MapShow(QWidget *parent , Qt::WFlags flags) : QWidget(parent, flags)
{
	createCentralWidget();
}

MapShow::~MapShow()
{

}

void MapShow::createCentralWidget()
{
    _widget = new ShowWidget;
	_sceneList = new TreeWidget;

	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins(1,1,1,1);
	
	QPushButton* button = new QPushButton("123");

	layout->addWidget(_sceneList, 0, 0, 1, 1);
	layout->addWidget(button, 1, 0, 1, 1);
    layout->addWidget(_widget, 0, 1, 1, 1);
	layout->setColumnMinimumWidth(0, 100);
	layout->setColumnMinimumWidth(1, 500);
	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 3);
	this->setLayout(layout);
	
	QObject::connect(_sceneList, SIGNAL(doubleClickStr(QString&)), this, SLOT(openScene(QString&)));

	QObject::connect(button, SIGNAL(clicked()), this, SLOT(openselece()));

}

void MapShow::openScene(QString& str)
{
        std::string modelPath = str.toStdString();
        bool success = _widget->getMessageHandler()->openFile(modelPath);

	if (success)
	{
		QMessageBox::warning (this, "提示", "加载成功");
	}
	else
	{
		QMessageBox::warning (this, "提示", "加载失败");
	}
}

void MapShow::openselece()
{
	QString str = QInputDialog::getText (this, "创建", "名称");
	unsigned int mask = 0x1;

	if (str == "0")
	{
		mask = mask<<0;
	}
	else
	if (str == "1")
	{
		mask = mask<<1;
	}
	else
	if (str == "2")
	{	
		mask = mask<<2;
	}
	else
	if (str == "3")
	{
		mask = mask<<3;
	}
	else
	{
		mask = 0x00ffffff;
	}

	mask|=SCENE_NODEMASK;

    _widget->getScene()->setCameraTravelMask(mask);
}

