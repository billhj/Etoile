#ifndef ETOILEMODULEAPP_H
#define ETOILEMODULEAPP_H

#include <QtGui/QMainWindow>
#include "ui_EtoileModuleApp.h"
#include "grapheditor/GraphEditor.h"
#include "Core.h"

class EtoileModuleApp : public QMainWindow
{
	Q_OBJECT

public:
	EtoileModuleApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~EtoileModuleApp();
	void createMenus();
	void initPlugins();
	
	QMenu* getMenuByName(QString name);

	void resizeEvent(QResizeEvent* event);
	public slots:
		void applyAction(QAction * action);
		void about();
		void changeStyle();
		void loadGraph();
		void saveGraph();
protected:
	//virtual void mouseReleaseEvent(QMouseEvent * event);
     void closeEvent(QCloseEvent *event);
	 void loadStyle();
	 void saveStyle();
	 void loadGraph(QString fileName);
private:
	Ui::EtoileModuleAppClass ui;
	QPalette* _pPalette;
	QPixmap* _pPixmap;

	GraphEditor* _editor;
	QMenu *fileMenu;
    QMenu *pluginMenu;
	QMenu *helpMenu;
	std::vector<QMenu *> _myMenuList;
	Etoile::Core _core;

	QString _stylefile;
	QString _graph;
};

#endif // ETOILEMODULEAPP_H
