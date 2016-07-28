
#include <MapFramework/EditLine.h>

using namespace MapGUI;

EditLine::EditLine(QWidget * parent, bool checkRange) : QLineEdit(parent),
    _checkRange(checkRange),
    _min(-180.0f),
    _max(180.0f),
    _value(0.0f)
{
    init();
}

void EditLine::init()
{
    this->setAcceptDrops(false);
    this->setAlignment(Qt::AlignHCenter);
    /*
    connect(this, SIGNAL(textChanged(QString)), SLOT(slotTextChanged(QString)));
    connect(this, SIGNAL(returnPressed()), SLOT(checkIsValid()));
    */
    QObject::connect( this, SIGNAL(editingFinished()), SLOT(checkIsValid()) );
    QObject::connect( this, SIGNAL(textChanged(const QString&)), SLOT(checkIsValid()) );
}

float EditLine::getValue()
{
    return _value;
}

void EditLine::checkIsValid()
{
    QString text = this->text();
    bool ok = false;
    _value = text.toFloat(&ok);
    if (ok)
    {
        if(_checkRange)
            _value = qBound(_min, _value, _max);

        QString str = QString::number(_value, 'f', 1);
        this->setText(str);
    }
    else
    {
        this->setText("0.0");
    }
}
