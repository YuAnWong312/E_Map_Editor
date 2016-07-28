
#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <osg/Switch>
#include <osg/Texture2D>

#include <iostream>

#include <MapFramework/Export.h>

namespace MapGUI
{
class MAPGUI_EXPORT ViewController : public osg::Referenced
{
public:
	ViewController(osg::Group* root, int width, int height);

	osg::Switch* createController(const std::string& nodeName, const osg::Vec3& corner,const osg::Vec3& widthVec,const osg::Vec3& heightVec, const std::string& firstImageName, const std::string& secondImageName);
	
	osg::Texture2D* createTexture(const std::string& imageName);
};
}
#endif
