
#include <MapFramework/CollectVisitor.h>
#include <osg/ValueObject>
#include <osg/MatrixTransform>

CollectVisitor::CollectVisitor() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
}

CollectVisitor::~CollectVisitor()
{

}

void CollectVisitor::apply(osg::Node& node)
{
	osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(&node);

	int type;
    if (mt && mt->getUserValue<int>("TYPE",type))
	{
        _nl.push_back(mt);
	}

	traverse(node);
}

void CollectVisitor::getSelections(osgScene::NodeList &nl, bool clearAfterCopy)
{
    nl = _nl;

    if(clearAfterCopy)
    {
        _nl.clear();
    }
}

