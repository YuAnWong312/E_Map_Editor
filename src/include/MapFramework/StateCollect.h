
#ifndef STATE_COLLECT
#define STATE_COLLECT

#include <map>
#include <string>
#include <vector>

#include <OpenThreads/Mutex>

#include <osg/Vec4>
#include <osg/observer_ptr>
#include <osg/Camera>
#include <osg/Switch>
#include <osg/Geometry>
#include <osgGA/GUIEventHandler>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>

#include <MapFramework/Export.h>

class MAPGUI_EXPORT ShowMessage : public osg::Referenced
{

public:
	ShowMessage(){}
	~ShowMessage(){}

	static ShowMessage* instence();
	struct Message
	{
		std::string tex;
		//QString tex;
		osg::Vec4 texColor;

		Message(std::string str, osg::Vec4 color) : tex(str) , texColor(color)
		{
		}
		Message() : tex(""), texColor(osg::Vec4(0.0, 0.0, 0.0, 0.0))
		{

		}
	};
	
	typedef std::map< std::string, Message > MessageList;
	typedef std::map< int, MessageList > Messages;

	void reset(); // clean _messages
	bool MessageToShow(int nodeType, std::string nodeName, 
		std::string show, osg::Vec4 color);

	std::string getMessage(int type, std::string nodeName, osg::Vec4& color);
	//get one message

	Messages getMessages();// get all message

protected:

	OpenThreads::Mutex _mutex;
	Messages _messages;
};

class MAPGUI_EXPORT ShowMessageHandler : public osgGA::GUIEventHandler
{
public:
	ShowMessageHandler();
    ~ShowMessageHandler(){}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void reset();
	
	void setShowKey(int key);

protected:
	void setUpHUDCamera(osgViewer::ViewerBase* viewer);
	void setUpScene(osgViewer::ViewerBase* viewer);

	osg::Geometry* createBackground(osg::Geode* geode, osg::Vec4 backColor, 
		osg::BoundingBox& preBB);

	void setWindowSize(int width, int height);

	bool                                _initialized;
    bool								_show;
    int                                 _keyShow;

	float								_cameraWidth;
	float								_cameraHeight;
	float								_characterSize;
	float								_blockMargin;
	float								_margin;

	osg::ref_ptr<osg::Camera>           _camera;
	osg::ref_ptr<osg::Switch>           _switch;
	osg::observer_ptr<ShowMessage>		_messages;
};

#endif
