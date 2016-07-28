
#include <MapFramework/ViewController.h>
#include <MapFramework/SceneManipulator.h>
#include <MapFramework/Define.h>

#include <osg/Camera>
#include <osg/observer_ptr>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

using namespace MapGUI;

void resize(osg::Camera* camera, int width, int height, int cameraWidth, int cameraHeight)
{
    int left = width - cameraWidth - 2;
    int bottom = height - cameraHeight - 4;

    //int left = width - cameraWidth;
    //int bottom = height - cameraHeight;

	left = left<0 ? 0:left;
	bottom = bottom<0 ? 0:bottom;

    camera->setViewport(left, bottom, cameraWidth, cameraHeight);
}

class ViewControllerCallback : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv)
	{
		osg::Camera* camera = dynamic_cast<osg::Camera*>(obj);


		if (camera)
		{
            /*
            static int needResize = 0;
            needResize++;
            if(needResize == 10 && osgGA::GUIEventAdapter::FRAME == ea.getEventType())
            {
                int width = ea.getWindowWidth();
                int height = ea.getWindowHeight();

                resize(camera, width, height, VIEWCONTROLLER_CAMERA_SIZE, VIEWCONTROLLER_CAMERA_SIZE);
            }
            else
            */
            if(osgGA::GUIEventAdapter::RESIZE == ea.getEventType())
			{
				int width = ea.getWindowWidth();
				int height = ea.getWindowHeight();

				resize(camera, width, height, VIEWCONTROLLER_CAMERA_SIZE, VIEWCONTROLLER_CAMERA_SIZE);
			}
			else if(osgGA::GUIEventAdapter::DOUBLECLICK == ea.getEventType())
			{
				return false;
			}
			else if(osgGA::GUIEventAdapter::PUSH == ea.getEventType() && osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON == ea.getButton())
			{
				osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
				if (viewer)
				{
					int x = ea.getX();
					int y = ea.getY();
					osgUtil::LineSegmentIntersector::Intersections intersectors;
					if( viewer->computeIntersections(x, y, intersectors, VIEWCONTROLLER_NODEMASK) )
					{
						osgUtil::LineSegmentIntersector::Intersections::iterator itr = intersectors.begin();
						for (itr; itr!=intersectors.end(); itr++)
						{
							osgUtil::LineSegmentIntersector::Intersection intersection;
							intersection = *itr;
							osg::NodePath np = intersection.nodePath;
							for (osg::NodePath::iterator itr = np.begin(); itr != np.end(); itr++)
							{
								osg::Switch* swi = dynamic_cast<osg::Switch*>(*itr);
								if (swi && swi->getNodeMask() == VIEWCONTROLLER_NODEMASK)
								{
									_preSwi = swi;
									if (osgGA::GUIEventAdapter::DOUBLECLICK == ea.getEventType())
									{
										return true;
									}
									if (osgGA::GUIEventAdapter::PUSH == ea.getEventType())
									{
										swi->setSingleChildOn(1);
										CustomerSceneManipulator* manipulator = dynamic_cast<CustomerSceneManipulator*>(viewer->getCameraManipulator());
										std::string nodeName = swi->getName();

										if ("left" == nodeName)
										{
											manipulator->setViewMode(CustomerSceneManipulator::LEFT);
										}
										else if("right" == nodeName)
										{
											manipulator->setViewMode(CustomerSceneManipulator::RIGHT);
										}
										else if("up" == nodeName)
										{
											manipulator->setViewMode(CustomerSceneManipulator::UP);
										}
										else if("down" == nodeName)
										{
											manipulator->setViewMode(CustomerSceneManipulator::DOWN);
										}
										else if("floor" == nodeName)
										{
											manipulator->setViewMode(CustomerSceneManipulator::HOME);
										}
										OSG_FATAL<<"swi have intersection 1"<<std::endl;
									}
								}
							}
						}
					}
				}
			}
			else if(osgGA::GUIEventAdapter::RELEASE == ea.getEventType())
			{
				if (_preSwi.valid())
				{
					_preSwi->setSingleChildOn(0);
					_preSwi = 0;
				}
			}
		}

		return osgGA::GUIEventHandler::handle(ea, aa);
	}

protected:
	osg::observer_ptr<osg::Switch> _preSwi;
};

ViewController::ViewController(osg::Group* root, int width, int height)
{
	if (root)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		//camera->setNodeMask(VIEWCONTROLLER_NODEMASK);
		camera->setAllowEventFocus(true);
		camera->setEventCallback(new ViewControllerCallback);
		camera->setRenderOrder(osg::Camera::POST_RENDER, 30);
		camera->setClearMask(GL_DEPTH_BUFFER_BIT);
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setProjectionMatrixAsOrtho(-3.0, 3.0, -3.0, 3.0, 0.0, 1.0);
		camera->setViewMatrixAsLookAt(osg::Vec3(0.0, 0.0, 1.0), osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 1.0, 0.0));
		//camera->setViewport(410, 445, VIEWCONTROLLER_CAMERA_SIZE, VIEWCONTROLLER_CAMERA_SIZE);
		resize(camera, width, height, VIEWCONTROLLER_CAMERA_SIZE, VIEWCONTROLLER_CAMERA_SIZE);

		root->addChild(camera.get());

		{
			std::string nodeNmae("left");
			osg::Vec3 corner(-3.0f, -1.0f, 0.0f);
			osg::Vec3 widthVec(2.0f, 0.0f, 0.0f);
			osg::Vec3 heightVec(0.0f, 2.0f, 0.0f);
			std::string firstImageName("./ViewController/left30%.png");
			std::string secondImageName("./ViewController/left100%.png");
			camera->addChild( createController(nodeNmae, corner, widthVec, heightVec, firstImageName, secondImageName) );
		}//left

		{
			std::string nodeNmae("right");
			osg::Vec3 corner(1.0f, -1.0f, 0.0f);
			osg::Vec3 widthVec(2.0f, 0.0f, 0.0f);
			osg::Vec3 heightVec(0.0f, 2.0f, 0.0f);
			std::string firstImageName("./ViewController/right30%.png");
			std::string secondImageName("./ViewController/right100%.png");
			camera->addChild( createController(nodeNmae, corner, widthVec, heightVec, firstImageName, secondImageName) );
		}//right

		{
			std::string nodeNmae("up");
			osg::Vec3 corner(-1.0f, 1.0f, 0.0f);
			osg::Vec3 widthVec(2.0f, 0.0f, 0.0f);
			osg::Vec3 heightVec(0.0f, 2.0f, 0.0f);
			std::string firstImageName("./ViewController/up30%.png");
			std::string secondImageName("./ViewController/up100%.png");
			camera->addChild( createController(nodeNmae, corner, widthVec, heightVec, firstImageName, secondImageName) );
		}//up

		{
			std::string nodeNmae("down");
			osg::Vec3 corner(-1.0f, -3.0f, 0.0f);
			osg::Vec3 widthVec(2.0f, 0.0f, 0.0f);
			osg::Vec3 heightVec(0.0f, 2.0f, 0.0f);
			std::string firstImageName("./ViewController/down30%.png");
			std::string secondImageName("./ViewController/down100%.png");
			camera->addChild( createController(nodeNmae, corner, widthVec, heightVec, firstImageName, secondImageName) );
		}//down

		{
			std::string nodeNmae("floor");
			osg::Vec3 corner(-1.0f, -1.0f, 0.0f);
			osg::Vec3 widthVec(2.0f, 0.0f, 0.0f);
			osg::Vec3 heightVec(0.0f, 2.0f, 0.0f);
			std::string firstImageName("./ViewController/floor30%.png");
			std::string secondImageName("./ViewController/floor100%.png");
			camera->addChild( createController(nodeNmae, corner, widthVec, heightVec, firstImageName, secondImageName) );
		}//floor
		
	}
}

osg::Switch* ViewController::createController(const std::string& nodeName, const osg::Vec3& corner,const osg::Vec3& widthVec,const osg::Vec3& heightVec, const std::string& firstImageName, const std::string& secondImageName)
{
	osg::Switch* swNode = new osg::Switch;
	swNode->setNodeMask(VIEWCONTROLLER_NODEMASK);
	swNode->setName(nodeName);

	osg::Geode* firstGeode = new osg::Geode;
	osg::Geode* secondGeode = new osg::Geode;

	swNode->addChild(firstGeode, true);
	swNode->addChild(secondGeode, false);

	{
		osg::Geometry* firstGeometry = osg::createTexturedQuadGeometry(corner, widthVec, heightVec);
		firstGeometry->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		firstGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		firstGeometry->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
		firstGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		firstGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture(firstImageName), osg::StateAttribute::ON);
		firstGeode->addDrawable(firstGeometry);
	}

	{
		osg::Geometry* secondGeometry = osg::createTexturedQuadGeometry(corner, widthVec, heightVec);
		secondGeometry->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		secondGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		secondGeometry->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
        secondGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		secondGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture(secondImageName), osg::StateAttribute::ON);
		secondGeode->addDrawable(secondGeometry);
	}
	
	return swNode;
}

osg::Texture2D* ViewController::createTexture(const std::string& imageName)
{
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(imageName);

	osg::Texture2D* texture = new osg::Texture2D;
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setImage( (image) );
	
	return texture;
}
