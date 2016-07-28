#ifndef DATABASE_EXPORT_H
#define DATABASE_EXPORT_H

#include <QtCore/qglobal.h>

#if	defined(DATABASE_STATIC)
#	define DATABASE_EXPORT
#elif defined(DATABASE_DYNAMIC)
#	define DATABASE_EXPORT Q_DECL_EXPORT
#else
#	define DATABASE_EXPORT Q_DECL_IMPORT
#endif
	
#endif // DATABASE_EXPORT_H
