
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

#include <Database/DatabaseConfig.h>

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
    DataBase::DatabaseConfig widget;
    //widget.setGeometry( 100, 100, 400, 300 );
    widget.resize(450, 500);
    widget.show();
    return a.exec();
}

