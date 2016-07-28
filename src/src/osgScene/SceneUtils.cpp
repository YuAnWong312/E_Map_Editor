
#include <osgScene/SceneUtils.h>
#include <osg/ValueObject>

namespace osgScene
{

osg::Matrix getWorldMatrix(osg::MatrixTransform* mt)
{
    osg::Matrix matrix;
    if(mt->getNumParents() > 0)
    {
        matrix = osg::computeLocalToWorld(mt->getParentalNodePaths()[0]);
    }
    return matrix;
}

osg::MatrixTransform* duplicate(osg::MatrixTransform* preMt)
{
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->setMatrix(preMt->getMatrix());

    int type = 0;
    preMt->getUserValue<int> ("TYPE", type);
    mt->setUserValue<int> ("TYPE", type);

    std::string name  = "undefined";
    preMt->getUserValue<std::string> ("NAME", name);
    mt->setUserValue<std::string> ("NAME", name);

    mt->setNodeMask(preMt->getNodeMask());

    unsigned int num = preMt->getNumChildren();
    for (unsigned int i = 0; i < num; i++)
    {
        mt->addChild(preMt->getChild(i));
    }

    return mt.release();
}

}
