
#include <osgScene/ReadAndShareCallback.h>

namespace osgScene
{

osgDB::ReaderWriter::ReadResult ReadAndShareCallback::readNode( const std::string& filename, const osgDB::Options* options )
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _shareMutex );
    osg::Node* node = getNodeByName( filename );
    if ( !node )
    {
        osgDB::ReaderWriter::ReadResult rr =
                osgDB::Registry::instance()->readNodeImplementation( filename, options );
        if ( rr.success() ) _nodeMap[filename] = rr.getNode();
        return rr;
    }
    else
        OSG_WARN << "[SHARING] The name " << filename << " is already added to the sharing list." << std::endl;
    return node;
}

osg::Node* ReadAndShareCallback::getNodeByName( const std::string& filename )
{
    NodeMap::iterator itr = _nodeMap.find(filename);
    if ( itr!=_nodeMap.end() ) return itr->second.get();
    return NULL;
}

}
