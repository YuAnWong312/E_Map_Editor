
#include <OpenThreads/ScopedLock>
#include <osg/ValueObject>
#include <osgViewer/Viewer>

#include <MapFramework/PickHandler.h>
#include <MapFramework/MapGUIMessageHandler.h>

PickHandler::PickHandler() : _intersectionMask( ~0x0 ){}

PickHandler::~PickHandler(){}

void PickHandler::setIntersectionMask(osg::Node::NodeMask mask)
{
    _intersectionMask = mask;
}

bool PickHandler::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{	
    if (ea.getHandled() == true)
    {
        return true;
    }

    osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if (!viewer)
    {
        return false;
    }

    int x = ea.getX ();
    int y = ea.getY ();

    switch(ea.getEventType ())
    {
    case osgGA::GUIEventAdapter::PUSH:
    {
        //更改与模型相关联的Selection
        if(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON == ea.getButtonMask())
        {
            osg::MatrixTransform* mt= pick(viewer, x, y);

            if (mt)
            {
                if ( (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL) == 0)
                {
                    _pickSelections.clear();
                }

                if (std::find(_pickSelections.begin(), _pickSelections.end(), mt) == _pickSelections.end())
                {
                    _pickSelections.push_back(mt);
                }

                MapGUI::MapGUIMessageHandler::instance()->setCurrentSelectionNodes(_pickSelections);
            }
        }
    }
        break;
    default:
        break;
    }
    return false;
}

osg::MatrixTransform* PickHandler::pick( osgViewer::Viewer* viewer, float x, float y )
{
    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);

    osgUtil::IntersectionVisitor iv( intersector.get() );
    iv.setTraversalMask(_intersectionMask);
    viewer->getCamera()->accept( iv );

    if ( intersector->containsIntersections() )
    {
        const osgUtil::LineSegmentIntersector::Intersection& result = *(intersector->getIntersections().begin());
        int size = result.nodePath.size ();

        osg::MatrixTransform* selection = 0;

        for(int index=0; index < size; index++)
        {
            selection = dynamic_cast<osg::MatrixTransform*>(result.nodePath[index]);

            if(cheakNode(selection))
                return selection;
        }
    }
    return 0;
}

void PickHandler::reset()
{
    _pickSelections.clear();
}

bool PickHandler::cheakNode(osg::MatrixTransform *mt)
{
    int type;
    if (mt && mt->getUserValue<int>("TYPE",type))
    {
        return true;
    }
    return false;
}
