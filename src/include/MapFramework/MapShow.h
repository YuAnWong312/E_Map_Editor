
#ifndef MAPSHOW_H
#define MAPSHOW_H

#include <QWidget>
#include <MapFramework/Widget.h>

class TreeWidget;

namespace MapGUI
{

class MAPGUI_EXPORT MapShow : public QWidget
{
	Q_OBJECT
public:
	MapShow(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~MapShow();

	virtual void createCentralWidget();

public slots:
	void openScene(QString& str);
	void openselece();

protected:
    TreeWidget*   _sceneList;
    ShowWidget*   _widget;
};

}
#endif
