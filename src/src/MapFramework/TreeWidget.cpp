
#include <MapFramework/TreeWidget.h>
#include <QtGui/QFileIconProvider>

TreeWidget::TreeWidget(QWidget *parent) :
        QWidget(parent)
{
    Init();
}

void TreeWidget::Init ()
{
    model = new QFileSystemModel(this);
	
    QString path_data = QDir::currentPath()+"/"+"SceneMap";
    //QString path_factory = tr("设备厂商");
	
    QDir dir;
    if(!dir.exists (path_data))
    {
        dir.mkdir (path_data);
        dir.cd (path_data);
        //dir.mkdir (path_factory);
    }
    //else
    //{
    //    dir.cd (path_data);
    //    if(!dir.exists (path_factory))
    //        dir.mkdir (path_factory);
    //}

    model->setRootPath(path_data);
    model->setFilter (QDir::Files | QDir::NoDotAndDotDot);
	
    treeView = new QTreeView;
    treeView->setModel (model);
    treeView->setHeaderHidden (true);
    treeView->setRootIndex (model->index(path_data));
    treeView->setColumnHidden (1,true);
    treeView->setColumnHidden (2,true);
    treeView->setColumnHidden (3,true);

	QFileIconProvider seekIcon;
	QIcon icon = seekIcon.icon(QFileIconProvider::Computer);
	//treeView->setIcon(0,icon);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget (treeView);
    this->setLayout (layout);

	
    //QObject::connect (treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getIndexPath()));
   // QObject::connect (treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(getIndexPath()));
    QObject::connect (treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClick()));
    QObject::connect (model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(checkDir()));
	
}

void TreeWidget::checkDir ()
{
    QString path_data = QDir::currentPath()+"/"+"SceneMap";
   // QString path_factory = tr("设备厂商");
    QDir dir;
    bool check = false;
    if(!dir.exists (path_data))
    {
        check = true;
        dir.mkdir (path_data);
        dir.cd (path_data);
        //dir.mkdir (path_factory);
    }
    //else
    //{
    //    dir.cd (path_data);
    //    if(!dir.exists (path_factory))
    //    {
    //        check = true;
    //        dir.mkdir (path_factory);
    //    }
    //}

    if(check)
    {
        if(model)
        {
            delete model;
            model = 0;
        }

        model = new QFileSystemModel(this);
        model->setFilter (QDir::Files | QDir::NoDotAndDotDot);
        model->setRootPath(path_data);
        treeView->setModel (model);
        treeView->setRootIndex (model->index(path_data));
    }
}

void TreeWidget::slotAdd ()
{
    checkDir ();

    QModelIndex index = treeView->currentIndex ();
    if(!index.isValid ())
    {
        QMessageBox::information (this, tr("警告"), tr("没有选中添加节点"));
        return;
    }

    int column = 0;
    QModelIndex tmp = index;
    while(tmp != treeView->rootIndex ())
    {
        tmp = tmp.parent ();
        column++;
    }
    qDebug ()<<column;

    if(column>4)
    {
        QMessageBox::information (this, tr("警告"), tr("节点添加位置错误"));
        return;
    }

    QString dirName = QInputDialog::getText (this, tr("创建"), tr("名称"));
    if(!dirName.isEmpty ())
    {
        model->mkdir (index, dirName);
    }
}

void TreeWidget::slotDelete ()
{
    QModelIndex index = treeView->currentIndex ();
    if(!index.isValid ())
    {
        QMessageBox::information (this, tr("警告"), tr("没有选中删除节点"));
        return;
    }

    int column = 0;
    QModelIndex tmp = index;
    while(tmp != treeView->rootIndex ())
    {
        tmp = tmp.parent ();
        column++;
    }

    qDebug ()<<column;
    if(column<2)
    {
        QMessageBox::information (this, tr("警告"), tr("节点不能删除"));
        return;
    }

    bool OK = false;
    OK = model->remove (index);
}

QString TreeWidget::getIndexPath ()
{
    QModelIndex index = treeView->currentIndex ();
    if(!index.isValid ())
    {
        return "";
    }

    QString str= model->fileInfo(index).path() + "/" + model->fileInfo(index).fileName(); /*+"/"+ model->fileName (index);*/
	//QString str= model->fileName(index);
    return str;
}

bool TreeWidget::checkPath ()
{
    //int column = 0;
    QModelIndex index = treeView->currentIndex ();
    if(!index.isValid ())
    {
        return false;
    }
    //QModelIndex tmp = index;
    //while(tmp != treeView->rootIndex ())
    //{
    //    tmp = tmp.parent ();
    //    column++;
    //}

    //if(column!=5)
    //{
    //    return false;
    //}
    return true;
}

void TreeWidget::doubleClick()
{
    QString str = getIndexPath();
    emit doubleClickStr(str);
}
