
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <MapShow.h>
#include <locale.h>

/*
#ifdef _MSC_VER
#    ifdef NDEBUG
#        pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#    else
#        pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#    endif
#endif
*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	setlocale(LC_ALL,".936");
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    MapGUI::MapShow widget;
    //MapGUI::ShowWidget widget;
    widget.show();
	return a.exec();
}

