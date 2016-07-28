
#ifndef MAPDRAGGER_H
#define MAPDRAGGER_H 1

#include <osgDragger/DraggerManager>
#include <osgDragger/MyDragger>

#include <QObject>
#include <MapFramework/Export.h>

namespace MapGUI
{
class MAPGUI_EXPORT CustomerMoveDragger : public QObject,  public osgDragger::TemplateMoveDragger<osgManipulator::Translate1DDragger, osgManipulator::Translate2DDragger>
{
	Q_OBJECT

public:
	CustomerMoveDragger(QObject *parent = 0);
    virtual ~CustomerMoveDragger(){}

	virtual void handleReleaseMessage(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

signals:
    void signalMoveFinish();

protected:

};

class MAPGUI_EXPORT CustomerRotateDragger : public QObject, public osgDragger::TemplateRotateDragger<osgManipulator::RotateCylinderDragger, osgManipulator::RotateSphereDragger>
{
	Q_OBJECT
public:
	CustomerRotateDragger(QObject *parent = 0);
    virtual ~CustomerRotateDragger(){}

	virtual void handleReleaseMessage(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

signals:
    void signalMoveFinish();

protected:

};

class MAPGUI_EXPORT CustomerDraggerManager : public QObject , public osgDragger::DraggerManager
{
	Q_OBJECT
public:
	static CustomerDraggerManager* Instence();

    CustomerDraggerManager() {}
    virtual ~CustomerDraggerManager() {}

signals:
    void signalMoveFinish();

protected:

};
}

#endif
