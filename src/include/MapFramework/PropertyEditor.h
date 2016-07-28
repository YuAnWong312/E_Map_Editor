#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H 1

#include <QWidget>
#include <MapFramework/Export.h>

class QPushButton;
class QLineEdit;

namespace MapGUI
{
class EditLine;
}

namespace MapGUI
{

class MAPGUI_EXPORT PropertyEditor : public QWidget
{
    Q_OBJECT

public:
    PropertyEditor(QWidget *parent = 0);

    void enable(int type);

    virtual QSize sizeHint() const;

public slots:
    void receivePropertyChange(QString str);
    void commitPropertyChange();
    void reset();

signals:
    void emitPropertyChange(QString str);

protected:
    virtual void init();

    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);

protected:

    EditLine* _posX;
    EditLine* _posY;
    EditLine* _posZ;

    EditLine* _rotX;
    EditLine* _rotY;
    EditLine* _rotZ;

    QPushButton* _commit;
};

}


#endif // PropertyEditor_H
