
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QStringList>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDragEnterEvent>

#include <MapFramework/EditLine.h>
#include <MapFramework/PropertyEditor.h>

using namespace MapGUI;

PropertyEditor::PropertyEditor(QWidget *parent)
    : QWidget(parent)
{
    init();
}

void PropertyEditor::init()
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->setAcceptDrops(true);
    this->setWindowTitle("属性");

    QLabel* lab_posX = new QLabel( "X" );
    QLabel* lab_posY = new QLabel( "Y" );
    QLabel* lab_posZ = new QLabel( "Z" );

    _posX = new EditLine;
    _posX->setMinMax(-10000.0f, 10000.0f);

    _posY = new EditLine;
    _posY->setMinMax(-10000.0f, 10000.0f);

    _posZ = new EditLine;
    _posZ->setMinMax(-10000.0f, 10000.0f);

    QGridLayout* posLayout = new QGridLayout;
    posLayout->addWidget(lab_posX, 0, 0);
    posLayout->addWidget(lab_posY, 0, 2);
    posLayout->addWidget(lab_posZ, 0, 4);
    posLayout->addWidget(_posX, 0, 1);
    posLayout->addWidget(_posY, 0, 3);
    posLayout->addWidget(_posZ, 0, 5);

    QGroupBox *posBox = new QGroupBox("位置");
    posBox->setLayout(posLayout);

    QLabel* lab_rotX = new QLabel( "X" );
    QLabel* lab_rotY = new QLabel( "Y" );
    QLabel* lab_rotZ = new QLabel( "Z" );

    _rotX = new EditLine;
    _rotX->setMinMax(-180.0f, 180.0f);

    _rotY = new EditLine;
    _rotY->setMinMax(-180.0f, 180.0f);

    _rotZ = new EditLine;
    _rotZ->setMinMax(-180.0f, 180.0f);

    QGridLayout* rotLayout = new QGridLayout;
    rotLayout->addWidget(lab_rotX, 0, 0);
    rotLayout->addWidget(lab_rotY, 0, 2);
    rotLayout->addWidget(lab_rotZ, 0, 4);
    rotLayout->addWidget(_rotX, 0, 1);
    rotLayout->addWidget(_rotY, 0, 3);
    rotLayout->addWidget(_rotZ, 0, 5);

    QGroupBox *rotBox = new QGroupBox("角度");
    rotBox->setLayout(rotLayout);

    _commit = new QPushButton("提交");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(1);
    mainLayout->addWidget(posBox);
    mainLayout->addWidget(rotBox);
    mainLayout->addWidget(_commit);
    mainLayout->addStretch(1);
    this->setLayout(mainLayout);

    QObject::connect(_commit, SIGNAL(pressed()), this, SLOT(commitPropertyChange()));

    /*
    QObject::connect(_objectName, SIGNAL(editingFinished()), this, SLOT(commitMT()));

    QObject::connect(_posX, SIGNAL(editingFinished()), this, SLOT(commitMT()));
    QObject::connect(_posY, SIGNAL(editingFinished()), this, SLOT(commitMT()));
    QObject::connect(_posZ, SIGNAL(editingFinished()), this, SLOT(commitMT()));


    QObject::connect(_rotX, SIGNAL(editingFinished()), this, SLOT(commitMT()));
    QObject::connect(_rotY, SIGNAL(editingFinished()), this, SLOT(commitMT()));
    QObject::connect(_rotZ, SIGNAL(editingFinished()), this, SLOT(commitMT()));
    */

    reset();
    enable(3);

}

void PropertyEditor::receivePropertyChange(QString str)
{
    QString parseStr = str;
    if ( !parseStr.isEmpty())
    {
        QStringList valueList = parseStr.split(";");
        if(valueList.size() == 6)
        {
            _posX->setText(valueList[0]);
            _posY->setText(valueList[1]);
            _posZ->setText(valueList[2]);

            _rotX->setText(valueList[3]);
            _rotY->setText(valueList[4]);
            _rotZ->setText(valueList[5]);

            enable(1);
        }
    }
    else
    {
        reset();
        enable(3);
    }
}

void PropertyEditor::commitPropertyChange()
{
    QString str = "";
    {
        float x = _posX->getValue();
        float y = _posY->getValue();
        float z = _posZ->getValue();
        str = str + QString("%1;%2;%3").arg(x,0,'f',1).arg(y,0,'f',1).arg(z,0,'f',1);
        //trans
    }

    {
        float x = _rotX->getValue();
        float y = _rotY->getValue();
        float z = _rotZ->getValue();
        str = str + ";" + QString("%1;%2;%3").arg(x,0,'f',1).arg(y,0,'f',1).arg(z,0,'f',1);
        //rotate
    }
    qDebug()<<"position:"<<str;
    emit emitPropertyChange(str);
}

void PropertyEditor::reset()
{
    _posX->setText("0.0");
    _posY->setText("0.0");
    _posZ->setText("0.0");

    _rotX->setText("0.0");
    _rotY->setText("0.0");
    _rotZ->setText("0.0");
}

void PropertyEditor::enable( int type )
{
    bool allEnable = false;
    bool databaseEnable = false;

    switch(type)
    {
    case 0:
    {
        allEnable = true;
        databaseEnable = false;
    }
        break;//modal
    case 1:
    {
        allEnable = true;
        databaseEnable = true;
    }
        break;//edit
    case 2:
    {
        allEnable = false;
        databaseEnable = true;
    }
        break;//all hide
    }

    _posX->setEnabled(allEnable);
    _posY->setEnabled(allEnable);
    _posZ->setEnabled(allEnable);

    _rotX->setEnabled(allEnable);
    _rotY->setEnabled(allEnable);
    _rotZ->setEnabled(allEnable);

    _commit->setEnabled(allEnable);

}

QSize PropertyEditor::sizeHint() const
{
    return QSize(0, 0);
}

void PropertyEditor::dropEvent( QDropEvent *event )
{
    event->setDropAction(Qt::MoveAction);
    event->acceptProposedAction();
}

void PropertyEditor::dragEnterEvent( QDragEnterEvent *event )
{
    event->setDropAction(Qt::MoveAction);
    event->acceptProposedAction();
}
