#ifndef MAPFRAMEWORK_EXPORT_H
#define MAPFRAMEWORK_EXPORT_H 1

#if	defined(MAPFRAMEWORK_STATIC)
# define MAPGUI_EXPORT
#elif defined(MAPFRAMEWORK_DYNAMIC)
# define MAPGUI_EXPORT __declspec(dllexport)
#else
# define MAPGUI_EXPORT __declspec(dllimport)
#endif
	
#endif // EXPORT_H
