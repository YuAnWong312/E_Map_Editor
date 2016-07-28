
#include <MapFramework/SceneManipulator.h>
#include <MapFramework/Define.h>

#include <osg/ComputeBoundsVisitor>
#include <osg/io_utils>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>


CustomerSceneManipulator::CustomerSceneManipulator(int flags) : osgGA::OrbitManipulator(flags)
{
	setMinimumDistance( 0.02, true );
	setAutoComputeHomePosition(true);
	setAllowThrow(false);
	setAnimationTime(1.0);
	this->setIgnoreHandledEventsMask(osgGA::GUIEventAdapter::KEY_Space);

	_viewMode = HOME;
}

CustomerSceneManipulator::~CustomerSceneManipulator()
{

}

bool CustomerSceneManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	switch( ea.getEventType() )
	{

	case osgGA::GUIEventAdapter::FRAME:
		return handleFrame( ea, us );

	case osgGA::GUIEventAdapter::RESIZE:
		return handleResize( ea, us );

	default:
		break;
	}

	if( ea.getHandled() )
		return false;

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		return handleMouseMove( ea, us );

	case osgGA::GUIEventAdapter::DRAG:
		return handleMouseDrag( ea, us );

	case osgGA::GUIEventAdapter::PUSH:
		return handleMousePush( ea, us );

	case osgGA::GUIEventAdapter::DOUBLECLICK:
		return handleDoubleClick(ea, us);

	case osgGA::GUIEventAdapter::RELEASE:
		return handleMouseRelease( ea, us );

	case osgGA::GUIEventAdapter::KEYDOWN:
		return handleKeyDown( ea, us );

	case osgGA::GUIEventAdapter::KEYUP:
		return handleKeyUp( ea, us );

	case osgGA::GUIEventAdapter::SCROLL:
		if( _flags & PROCESS_MOUSE_WHEEL )
			return handleMouseWheel( ea, us );
		else
			return false;

	default:
		return false;
	}
}

bool CustomerSceneManipulator::performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	return osgGA::OrbitManipulator::performMovementLeftMouseButton(eventTimeDelta, dx, dy);

}

bool CustomerSceneManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
	panModel( dx*scale, dy*scale );
	return true;
}

bool CustomerSceneManipulator::handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	flushMouseEventStack();
	addMouseEvent( ea );

	if( performMovement() )
		us.requestRedraw();

	us.requestContinuousUpdate( false );
	_thrown = false;

	return true;
}

bool CustomerSceneManipulator::handleDoubleClick( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	bool out = false;
	if ( osgGA::GUIEventAdapter::DOUBLECLICK == ea.getEventType())
	{
		if (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON == ea.getButtonMask())
		{
			if( getAnimationTime() <= 0. )
			{
				// center by mouse intersection (no animation)
				out = setCenterByMousePointerIntersection( ea, us );
			}
			else
			{
				// start new animation only if there is no animation in progress
				if( !isAnimating() )
					out = startAnimationByMousePointerIntersection( ea, us );

			}
			OSG_INFO<<"left button double click"<<std::endl;
		}
	}

	return out;
}

bool CustomerSceneManipulator::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	switch( sm )
	{
		// mouse scroll up event
	case osgGA::GUIEventAdapter::SCROLL_UP:
		{
			// perform zoom
			zoomModel( -_wheelZoomFactor, false ); // scale little
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// mouse scroll down event
	case osgGA::GUIEventAdapter::SCROLL_DOWN:
		{
			// perform zoom
			zoomModel( _wheelZoomFactor, false ); // scale big
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// unhandled mouse scrolling motion
	default:
		return false;
	}
}

bool CustomerSceneManipulator::performMovement()
{
	// return if less then two events have been added
	if( _ga_t0.get() == NULL || _ga_t1.get() == NULL )
		return false;

	// get delta time
	double eventTimeDelta = _ga_t0->getTime() - _ga_t1->getTime();
	if( eventTimeDelta < 0. )
	{
		OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
		eventTimeDelta = 0.;
	}

	// get deltaX and deltaY
	float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
	float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();

	// return if there is no movement.
	if( dx == 0. && dy == 0. )
		return false;


	// call appropriate methods
	unsigned int buttonMask = _ga_t1->getButtonMask();
	if( buttonMask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
	{
		//return performMovementLeftMouseButton( eventTimeDelta, dx, dy );
	}
	else if( buttonMask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON )
	{
		return performMovementLeftMouseButton( eventTimeDelta, dx, dy ); // move
	}
	else if( buttonMask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
	{
		return performMovementRightMouseButton( eventTimeDelta, dx, dy ); // rotate
	}

	return false;
}

void CustomerSceneManipulator::computeHomePosition( const osg::Camera *camera /* = NULL */, bool useBoundingBox /* = false */ )
{
	if (getNode())
	{
		osg::BoundingSphere boundingSphere;

		OSG_INFO<<" CameraManipulator::computeHomePosition("<<camera<<", "<<useBoundingBox<<")"<<std::endl;

		if (useBoundingBox)
		{
			// compute bounding box
			// (bounding box computes model center more precisely than bounding sphere)
			osg::ComputeBoundsVisitor cbVisitor;
			getNode()->accept(cbVisitor);
			osg::BoundingBox &bb = cbVisitor.getBoundingBox();

			if (bb.valid()) 
				boundingSphere.expandBy(bb);
			else 
				boundingSphere = getNode()->getBound();
		}
		else
		{
			// compute bounding sphere
			boundingSphere = getNode()->getBound();
		}

		OSG_INFO<<" boundingSphere.center() = ("<<boundingSphere.center()<<")"<<std::endl;
		OSG_INFO<<" boundingSphere.radius() = "<<boundingSphere.radius()<<std::endl;

		// set dist to default
		double dist = 1.5 * boundingSphere.radius();
		
		camera = getCamera();
		if (camera)
		{
			// try to compute dist from frustrum
			double left,right,bottom,top,zNear,zFar;
			if (camera->getProjectionMatrixAsFrustum(left,right,bottom,top,zNear,zFar))
			{
				double vertical2 = fabs(right - left) / zNear / 2.;
				double horizontal2 = fabs(top - bottom) / zNear / 2.;
				double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
				double viewAngle = atan2(dim,1.);
				dist = boundingSphere.radius() / sin(viewAngle);
			}
			else
			{
				// try to compute dist from ortho
				if (camera->getProjectionMatrixAsOrtho(left,right,bottom,top,zNear,zFar))
				{
					dist = fabs(zFar - zNear) / 2.;
				}
			}
		}
		

		// set home position

		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		switch(_viewMode)
		{
		case HOME:
			{
				x = dist;
				y = -dist;
				z = dist;
			}
			break;
		case LEFT:
			{
				x = -dist;
				y = 0.0;
				z = 0.0;
			}
			break;
		case RIGHT:
			{
				x = dist;
				y = 0.0;
				z = 0.0;
			}
			break;
		case UP:
			{
				x = 0.0;
				y = -dist;
				z = 0.0;
			}
			break;
		case DOWN:
			{
				x = 0.0;
				y = dist;
				z = 0.0;
			}
			break;
		default:
			break;
		}
		OSG_FATAL<<" xyz = "<<x<<" "<<y<<" "<<z<<std::endl;
		setHomePosition(
			boundingSphere.center() + osg::Vec3d(x, y, z),
			boundingSphere.center(),
			osg::Vec3d(0.0f,0.0f,1.0f),
			_autoComputeHomePosition
			);

	}
}

bool CustomerSceneManipulator::setCenterByMousePointerIntersection( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osg::View* view = us.asView();
	if( !view )
		return false;

	osg::Camera *camera = view->getCamera();
	if( !camera )
		return false;

	// prepare variables
	float x = ( ea.getX() - ea.getXmin() ) / ( ea.getXmax() - ea.getXmin() );
	float y = ( ea.getY() - ea.getYmin() ) / ( ea.getYmax() - ea.getYmin() );
	osgUtil::Intersector::CoordinateFrame cf;
	osg::Viewport *vp = camera->getViewport();
	if( vp ) {
		cf = osgUtil::Intersector::WINDOW;
		x *= vp->width();
		y *= vp->height();
	} else
		cf = osgUtil::Intersector::PROJECTION;

	// perform intersection computation
	osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector( cf, x, y );
	osgUtil::IntersectionVisitor iv( picker.get() );
	iv.setTraversalMask(PICK_HANDLEMASK);
	camera->accept( iv );

	// return on no intersections
	if( !picker->containsIntersections() )
		return false;

	// get all intersections
	osgUtil::LineSegmentIntersector::Intersections& intersections = picker->getIntersections();

	// get current transformation
	osg::Vec3d eye, oldCenter, up;
	getTransformation( eye, oldCenter, up );

	// new center
	osg::Vec3d newCenter = (*intersections.begin()).getWorldIntersectPoint();

	// make vertical axis correction
	if( getVerticalAxisFixed() )
	{

		osg::CoordinateFrame coordinateFrame = getCoordinateFrame( newCenter );
		osg::Vec3d localUp = getUpVector( coordinateFrame );

		fixVerticalAxis( newCenter - eye, up, up, localUp, true );

	}

	// set the new center
	setTransformation( eye, newCenter, up );

	//centerMousePointer( ea, us );

	return true;
}

void CustomerSceneManipulator::zoomModel( const float dy, bool pushForwardIfNeeded /*= true */ )
{
	// scale
	float scale = 1.0f + dy;

	if( _node.get() )
	{
		const osg::BoundingSphere& boundingSphere = _node->getBound();
		_modelSize = boundingSphere.radius();
	}
	else
	{
		_modelSize = 0.;
	}

	/*
	// minimum distance
	float minDist = _minimumDistance;
	//if( getRelativeFlag( _minimumDistanceFlagIndex ) )
	//	minDist *= _modelSize;

	minDist = _modelSize * 0.2f;
	float maxDist = _modelSize * 5.4f;
	
	float dist = _distance*scale;
	if (dist >= maxDist)
	{
		_distance = maxDist;
	}
	else
		if ( (dist < maxDist)&&(dist >= minDist))
		{
			_distance = dist;
		}
		else
		{
			if( pushForwardIfNeeded )
			{
				// push the camera forward
				float scale = -_distance;
				osg::Matrixd rotation_matrix( _rotation );
				osg::Vec3d dv = (osg::Vec3d( 0.0f, 0.0f, -1.0f ) * rotation_matrix) * (dy * scale);
				_center += dv;
			}
			else
			{
				// set distance on its minimum value
				_distance = minDist;
			}
		}
	*/

	// minimum distance
	float minDist = _minimumDistance;
	if( getRelativeFlag( _minimumDistanceFlagIndex ) )
		minDist *= _modelSize;

	if( _distance*scale > minDist )
	{
		// regular zoom
		_distance *= scale;
	}
	else
	{
		if( pushForwardIfNeeded )
		{
			// push the camera forward
			float scale = -_distance;
			osg::Matrixd rotation_matrix( _rotation );
			osg::Vec3d dv = (osg::Vec3d( 0.0f, 0.0f, -1.0f ) * rotation_matrix) * (dy * scale);
			_center += dv;
		}
		else
		{
			// set distance on its minimum value
			_distance = minDist;
		}
	}
}

bool CustomerSceneManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	return false;
}
