
#ifndef SCENEUTILS_H
#define SCENEUTILS_H 1

#include <osg/observer_ptr>
#include <osg/MatrixTransform>
#include <vector>
#include <osgScene/Export.h>

namespace osgScene
{
typedef std::vector< osg::observer_ptr<osg::MatrixTransform> > SelectionList;
typedef std::vector< osg::ref_ptr<osg::MatrixTransform> > NodeList;
extern OSGSCENE_EXPORT osg::Matrix getWorldMatrix(osg::MatrixTransform* mt);
extern OSGSCENE_EXPORT osg::MatrixTransform* duplicate(osg::MatrixTransform* preMt);
}

#endif
