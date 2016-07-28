
#include <MapFramework/MapDragger.h>

using namespace MapGUI;
static osg::ref_ptr<CustomerDraggerManager> interface;

CustomerDraggerManager* CustomerDraggerManager::Instence()
{
	if (!interface.valid())
	{
		interface = new CustomerDraggerManager;
	}
	return interface.get();
}

CustomerMoveDragger::CustomerMoveDragger(QObject *parent /* = 0 */) : QObject(parent)
{
	
}

void CustomerMoveDragger::handleReleaseMessage( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    emit signalMoveFinish();
}

CustomerRotateDragger::CustomerRotateDragger(QObject *parent /* = 0 */) : QObject(parent)
{
	
}

void CustomerRotateDragger::handleReleaseMessage( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    emit signalMoveFinish();
}
