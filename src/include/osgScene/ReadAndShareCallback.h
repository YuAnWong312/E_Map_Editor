
#ifndef READANDSHARECALLBACK_H
#define READANDSHARECALLBACK_H 1

#include <osgDB/Registry>
#include <osgScene/Export.h>

namespace osgScene
{

class OSGSCENE_EXPORT ReadAndShareCallback : public osgDB::ReadFileCallback
{
public:
    virtual osgDB::ReaderWriter::ReadResult readNode( const std::string& filename, const osgDB::Options* options );

protected:
    osg::Node* getNodeByName( const std::string& filename );

protected:
    typedef std::map<std::string, osg::ref_ptr<osg::Node> > NodeMap;
    NodeMap _nodeMap;
    OpenThreads::Mutex _shareMutex;
};

}
#endif
