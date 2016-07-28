#ifndef OSGSCENE_H
#define OSGSCENE_H 1

#if	defined(OSGSCENE_STATIC)
# define OSGSCENE_EXPORT
#elif defined(OSGSCENE_DYNAMIC)
# define OSGSCENE_EXPORT __declspec(dllexport)
#else
# define OSGSCENE_EXPORT __declspec(dllimport)
#endif
	
#endif // EXPORT_H
