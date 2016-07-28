#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QtGui>
#include <QVector>
class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    TreeWidget(QWidget *parent = 0);

    void Init();

signals:
    void doubleClickStr(QString& str);

public slots:
	void doubleClick();
    void slotAdd ();
    void slotDelete();

    QString getIndexPath();

    bool checkPath();
    void checkDir();

private:
    QFileSystemModel* model;
    QTreeView* treeView;

};

#endif // TREEWIDGET_H
