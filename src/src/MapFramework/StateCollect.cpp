
#include <MapFramework/StateCollect.h>

#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osgText/Font>
#include <osgText/Text>
#include <osgViewer/Renderer>

#include <iostream>
#include <sstream>

void MultibyteToWide(osgText::Text* text, const std::string& str)
{
	std::string tmpStr = str;
	int requireSize = mbstowcs(NULL, tmpStr.c_str(), 0);
	wchar_t* wtext = new wchar_t[requireSize+1];
	mbstowcs(wtext, tmpStr.c_str(), requireSize+1);
	text->setText(wtext);
	delete wtext;
}
/*
std::wstring MultibyteToWide(const std::string& multibyte)
{
	size_t length = multibyte.length();
	if (length == 0)
		return std::wstring();

	wchar_t * wide = new wchar_t[multibyte.length()*2+2];
	if (wide == NULL)
		return std::wstring();
	int ret = MultiByteToWideChar(CP_ACP, 0, multibyte.c_str(), (int)multibyte.size(), wide, (int)length*2 - 1);
	wide[ret] = 0;
	std::wstring str = wide;
	delete[] wide;
	return str;
}
*/

ShowMessage* ShowMessage::instence()
{
	static osg::ref_ptr<ShowMessage> sm = new ShowMessage;
	return sm.get();
}

bool ShowMessage::MessageToShow(int nodeType, std::string nodeName, std::string show, osg::Vec4 color)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);

	Messages::iterator itr_p = _messages.find(nodeType);
	if (itr_p != _messages.end())
	{
		MessageList::iterator itr = itr_p->second.find(nodeName);
		if ( itr != itr_p->second.end() )
		{
			itr->second.tex = show;
			itr->second.texColor = color;
		}// update
		else
		{
			(itr_p->second)[nodeName] = Message(show, color);
		}// new
	}
	else
	{
		MessageList ml;
		ml[nodeName] = Message(show, color);
		_messages[nodeType]= ml;

	}// new

	return true;
}

std::string ShowMessage::getMessage(int type, std::string nodeName, osg::Vec4& color)
{
	std::string message = "";
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	Messages::iterator itr_p = _messages.find(type);
	if (itr_p == _messages.end())//null
	{
		return message;
	}
	else
	{
		MessageList::iterator itr = itr_p->second.find(nodeName);
		if (itr != itr_p->second.end())//null
		{
			Message msg = itr->second;
			message = msg.tex;
			color = msg.texColor;
		}
		else
		{
			return message;
		}
	}
	return message;
}

ShowMessage::Messages ShowMessage::getMessages()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _messages;
}

void ShowMessage::reset()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	_messages.clear();
}


//////////////////////////////////
//////////////////////////////////
int num=0;
class TextUpdate : public osg::Drawable::DrawCallback
{
public:
	TextUpdate(int type, std::string str) : 
	  _type(type), _nodeName(str)
	{
	}

    virtual void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const
	{
		osgText::Text* text = (osgText::Text*)drawable;
		osg::Vec4 color(0.0, 0.0, 1.0, 1.0);
		std::string str = 
			ShowMessage::instence()->getMessage(_type, _nodeName, color);
		////////
		////////
		/*
		num++;
		std::stringstream ss;
		ss<<num;
		str = ss.str();

		for (unsigned int i=0; i<text->getText().size(); i++ )
		{
			if((i+1)%7 == 0)
			{
				//str.clear();
				str = "1";
				break;
			}
			str += "1";
		}
		*/
		///////
		///////
		//std::wstring wstr = MultibyteToWide(str);
		//text->setText( wstr.c_str() );
		MultibyteToWide(text, str);
		text->setColor(color);

		/*
		if (_mt.valid())
		{
			osg::ComputeBoundsVisitor bbVisitor;
			_mt->accept(bbVisitor);
			osg::BoundingBox preBB = bbVisitor.getBoundingBox();
			osg::BoundingBox geoBB = drawable->getBound();
			bool intersect = preBB.intersects(geoBB);
			if (intersect)
			{
				osg::Vec3 pos = text->getPosition();
				pos.x() = preBB.xMax() + _margin;
				text->setPosition(pos);
				text->dirtyBound();
			}
		}
		*/
		
		text->drawImplementation(renderInfo);
	}

protected:
	int _type;
	std::string _nodeName;
};

class BackgroundGeometryUpdate : public osg::Drawable::DrawCallback
{
public:
	BackgroundGeometryUpdate(osg::Geode* geode, float margin) : 
	  _parentGeode(geode) , _margin(margin)
	{
	}

	virtual void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const
	{
		if (_parentGeode.valid())
		{
			osg::BoundingBox parentBB /*= _parentGeode->getBoundingBox()*/;
			for (unsigned int i=0; i<_parentGeode->getNumDrawables()-1; i++)
			{
				parentBB.expandBy( _parentGeode->getDrawable(i)->getBound() );
			}
			osg::BoundingBox backgroundBB = drawable->getBound();

			if (parentBB.xMin() != backgroundBB.xMin() 
				|| parentBB.xMax() != backgroundBB.xMax())
			{
				osg::Geometry* geometry  = (osg::Geometry*)(drawable);
				osg::Vec3Array* vertices = new osg::Vec3Array;
				float depth = parentBB.zMin()-0.1;

				float xMin = parentBB.xMin() - _margin;
				float xMax = parentBB.xMax() + _margin;
				float yMin = parentBB.yMin() - _margin;
				float yMax = parentBB.yMax() + _margin;

				vertices->push_back(osg::Vec3(xMin, yMax, depth));
				vertices->push_back(osg::Vec3(xMin, yMin, depth));
				vertices->push_back(osg::Vec3(xMax, yMin, depth));
				vertices->push_back(osg::Vec3(xMax, yMax, depth));
				geometry->setVertexArray(vertices);
				geometry->dirtyBound();
				
			}

			drawable->drawImplementation(renderInfo);
		}
	}

protected:
	osg::observer_ptr<osg::Geode> _parentGeode;
	float _margin;
};

class BlockPositionUpdate : public osg::NodeCallback
{
public:
	BlockPositionUpdate(osg::MatrixTransform* preMT, osg::MatrixTransform* selfMT, float margin)
		: _preMT(preMT) , _selfMT(selfMT), _margin(margin)
	{
		
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
		{
			if( _preMT.valid() && _selfMT.valid() )
			{
				osg::ComputeBoundsVisitor bbVisitor;
				_preMT->accept(bbVisitor);
				osg::BoundingBox preBB = bbVisitor.getBoundingBox();

				osg::Vec3 trans = _selfMT->getMatrix().getTrans();
				trans.x() = preBB.xMax() + _margin;
				_selfMT->setMatrix(osg::Matrix::translate(trans));
			}
		}
		
		traverse(node,nv);
	}

protected:
	osg::observer_ptr<osg::MatrixTransform> _preMT;
	osg::observer_ptr<osg::MatrixTransform> _selfMT;
	float _margin;
};

ShowMessageHandler::ShowMessageHandler() : 
        _initialized(false) ,
        _show(false) ,
		_keyShow('h') , 
		_cameraWidth(1280.0f) , 
		_cameraHeight(1024.0f) , 
		_characterSize(20.0f) ,
		_blockMargin(12.0f) ,
		_margin(5.0f)
{

	//test add Message
	for(int i=0; i<5; i++)
	{
		ShowMessage::instence()->MessageToShow(i, "空调", "1", osg::Vec4(1.0,0.0,0.0,1.0));
		ShowMessage::instence()->MessageToShow(i, "温度", "2", osg::Vec4(0.0,1.0,0.0,1.0));
		ShowMessage::instence()->MessageToShow(i, "湿度", "2", osg::Vec4(0.0,0.0,1.0,1.0));
	}

	_camera = new osg::Camera;
	_camera->setRenderer(new osgViewer::Renderer(_camera.get()));
	_camera->setProjectionResizePolicy(osg::Camera::FIXED);
}

void ShowMessageHandler::reset()
{
	_initialized = false;
	_camera->setGraphicsContext(0);
	_camera->removeChildren(0, _camera->getNumChildren());
}

bool ShowMessageHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getHandled()) return false;

	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return false;

	osgViewer::ViewerBase* viewer = view->getViewerBase();
	if (!viewer) return false;

	switch(ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			if (ea.getKey()==_keyShow)
			{
				if (!_initialized)
				{
					setUpHUDCamera(viewer);
					setUpScene(viewer);
				}

				_show = !_show;

				if (_show)
				{
					_camera->setNodeMask(0xffffffff);
				}
				else
				{
					_camera->setNodeMask(0);
				}
				return true;
			}
			else
			{

				ShowMessage::instence()->MessageToShow(0, "温度", "12",osg::Vec4(1.0,0.0,0.0,1.0));
			}
		}
		break;
	case(osgGA::GUIEventAdapter::RESIZE):
		{
			setWindowSize(ea.getWindowWidth(), ea.getWindowHeight());
		}
		break;
	default: break;
	}

	return false;
}

void ShowMessageHandler::setUpHUDCamera(osgViewer::ViewerBase* viewer)
{
	osg::GraphicsContext* context = dynamic_cast<osgViewer::GraphicsWindow*>(_camera->getGraphicsContext());

	if (!context)
	{
		osgViewer::Viewer::Windows windows;
		viewer->getWindows(windows);

		if (!windows.empty()) context = windows.front();
		else
		{
			context = _camera->getGraphicsContext();

			if (!context)
			{
				osgViewer::Viewer::Contexts contexts;
				viewer->getContexts(contexts);

				if (contexts.empty()) return;

				context = contexts.front();
			}
		}
	}

	_camera->setGraphicsContext(context);

	_camera->setRenderOrder(osg::Camera::POST_RENDER, 10);
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());

	setWindowSize(context->getTraits()->width, context->getTraits()->height);

	_camera->setClearMask(0);
	_camera->setRenderer(new osgViewer::Renderer(_camera.get()));
	osgViewer::Viewer* v = dynamic_cast<osgViewer::Viewer*>(viewer);
	v->getSceneData()->asGroup()->addChild(_camera.get());
	_initialized = true;
}

void ShowMessageHandler::setUpScene(osgViewer::ViewerBase* viewer)
{
	_switch = new osg::Switch;// geode root
	_camera->addChild(_switch.get());

	osg::StateSet* stateset = _switch->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateset->setAttribute(new osg::PolygonMode(), osg::StateAttribute::PROTECTED);

	std::string fontStr("./font/simhei.ttf");
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile(fontStr);

	osg::Vec4 backgroundColor(0.0, 0.0, 0.0f, 0.3);
	int pageNum =0;
	osg::BoundingBox bb;
	bb.init();

	osg::Geode* preGeode = 0;
	osg::MatrixTransform* preMT = 0;
	ShowMessage::Messages messages = ShowMessage::instence()->getMessages();


	float tmpleftPos = _blockMargin;
	float tmptopPos = _cameraHeight - 1.0f;

    ShowMessage::Messages::const_iterator itr_p = messages.begin();
	for ( itr_p; itr_p != messages.end(); itr_p++)
	{
		ShowMessage::MessageList ml = itr_p->second;
		ShowMessage::MessageList::const_iterator itr = ml.begin();
		
		int type = itr_p->first;
		osg::Geode* geode = new osg::Geode;
		osg::MatrixTransform* mt = new osg::MatrixTransform;
		mt->addChild(geode);
		_switch->addChild(mt, true);// new type <-> new mt
		
		osg::Vec3 mtPos;

		if(pageNum == 0)
		{
			mtPos = osg::Vec3( tmpleftPos, tmptopPos, 0.0f );
		}
		else
		{
			tmpleftPos += bb.xMax() + _blockMargin;
			mtPos = osg::Vec3( tmpleftPos, tmptopPos, 0.0f );
		}
		mt->setMatrix( osg::Matrix::translate(mtPos) );

		osg::Vec3 texStartPos(_margin, -(_characterSize + _margin), 0.0f);
		for (itr; itr != ml.end(); itr++)
		{
			std::string show_name = itr->first;				//node name
			ShowMessage::Message message = itr->second;	
			std::string show_value = message.tex;			//show message
			osg::Vec4 color = message.texColor;				//texcolor

			osg::ref_ptr<osgText::Text> label = new osgText::Text;
			geode->addDrawable( label.get() );

			label->setColor(color);
			label->setFont(font);
			label->setCharacterSize(_characterSize);
			label->setPosition(texStartPos);
			//std::wstring str = MultibyteToWide(show_name+" : ");
			//label->setText( str.c_str() );
			std::string tmpStr = show_name + " : ";
			MultibyteToWide((label), tmpStr);
			texStartPos.x() = label->getBound().xMax();

			osg::ref_ptr<osgText::Text> Value = new osgText::Text;
			geode->addDrawable( Value.get() );

			Value->setColor(color);
			Value->setFont(font);
			Value->setCharacterSize(_characterSize);
			Value->setPosition(texStartPos);
			//str = MultibyteToWide(show_value);
			//Value->setText( str.c_str() );
			MultibyteToWide((Value), show_value);
			Value->setDrawCallback(new TextUpdate(type, show_name));
			texStartPos.x() = _margin;
			texStartPos.y() -= (_characterSize + _margin);
		}
		mt->setUpdateCallback( new BlockPositionUpdate(preMT, mt, _blockMargin) );
		createBackground(geode, backgroundColor, bb);
		preGeode = geode;
		preMT = mt;
		pageNum++;
	}

	/*
	osg::BoundingBox bb = geode->getBoundingBox();
	if (bb.valid())
	{
		float width = bb.xMax() - bb.xMin();
		float height = bb.yMax() - bb.yMin();
		float ratio = 1.0;
		if (width > 1024.0f) ratio = 1024.0f/width;
		if (height*ratio > 800.0f) ratio = 800.0f/height;

		_camera->setViewMatrix(osg::Matrix::translate(-bb.center()) *
			osg::Matrix::scale(ratio,ratio,ratio) *
			osg::Matrix::translate(osg::Vec3(640.0f, 520.0f, 0.0f)));
	}
	*/
}

void ShowMessageHandler::setWindowSize(int width, int height)
{
	if (width <= 0 || height <= 0)
		return;

	float _statsWidth = _cameraWidth;
	float _statsHeight = _cameraHeight;
	_camera->setViewport(0, 0, width, height);
	if (fabs(height*_statsWidth) <= fabs(width*_statsHeight))
	{
		_camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0,width*_statsHeight/height,0.0,_statsHeight));
	}
	else
	{
		_camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0,_statsWidth,_statsHeight-height*_statsWidth/width,_statsHeight));
	}
}

osg::Geometry* ShowMessageHandler::createBackground(osg::Geode* geode, osg::Vec4 backColor, osg::BoundingBox& preBB)
{
	osg::BoundingBox bb;
	for(unsigned int i=0; i<geode->getNumDrawables(); ++i)
	{
		bb.expandBy(geode->getDrawable(i)->getBound());
	}

	osg::Geometry* geom = new osg::Geometry;

	osg::Vec3Array* vertices = new osg::Vec3Array;
	float depth = bb.zMin()-0.1;

	float xMin = bb.xMin() - _margin ;
	float xMax = bb.xMax() + _margin ;
	float yMin = bb.yMin() - _margin ;
	float yMax = bb.yMax() + _margin ;

	vertices->push_back(osg::Vec3(xMin, yMax, depth));
	vertices->push_back(osg::Vec3(xMin, yMin, depth));
	vertices->push_back(osg::Vec3(xMax, yMin, depth));
	vertices->push_back(osg::Vec3(xMax, yMax, depth));
	geom->setVertexArray(vertices);

	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(backColor);
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

	geom->setDrawCallback(new BackgroundGeometryUpdate(geode, _margin));
	geom->setUseDisplayList(false);
	osg::StateSet* stateset = geom->getOrCreateStateSet();
	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
	//stateset->setAttribute(new osg::PolygonOffset(1.0f,1.0f),osg::StateAttribute::ON);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geode->addDrawable(geom);

	preBB = geode->getBoundingBox();
	return geom;
}
