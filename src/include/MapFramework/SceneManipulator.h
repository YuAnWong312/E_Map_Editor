
#ifndef SCENEMANIPULATOR_H
#define SCENEMANIPULATOR_H

#include <osgGA/OrbitManipulator>

#include <MapFramework/Export.h>

class MAPGUI_EXPORT CustomerSceneManipulator : public osgGA::OrbitManipulator
{

public:
	CustomerSceneManipulator(int flags = osgGA::OrbitManipulator::DEFAULT_SETTINGS);
	virtual ~CustomerSceneManipulator();
	
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );

	virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );

	virtual void computeHomePosition(const osg::Camera *camera /* = NULL */, bool useBoundingBox /* = false */);

	virtual bool setCenterByMousePointerIntersection( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	inline void setCamera(osg::Camera* cam)
	{
		_camera = cam;
	}

	inline osg::Camera* getCamera()
	{
		return _camera.get();
	}

	enum VIEWMODE
	{
		HOME=1,
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};

	inline void setViewMode(VIEWMODE mode)
	{
		_viewMode = mode;
		home(0.0);
	}

protected:

	virtual bool handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
    virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	virtual bool handleDoubleClick( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual void zoomModel( const float dy, bool pushForwardIfNeeded = true );

	virtual bool performMovement();

protected:
	osg::observer_ptr<osg::Camera> _camera;
	int _viewMode;
};

#endif // SCENEMANIPULATOR_H
