#ifndef OPENFILEPLUGIN_H
#define OPENFILEPLUGIN_H

#include <QDockWidget>
#include "ui_OpenFilePlugin.h"
#include "module/EPlugin.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/GraphScene.h"

typedef Etoile::StringOutputSocket OpenFileStringSocket;

class OpenFilePlugin : public QDockWidget, public Etoile::EPlugin, public Etoile::SocketNode 
{
	Q_OBJECT

public:
	OpenFilePlugin(const std::string name, QWidget *parent = 0);
	~OpenFilePlugin();
	virtual void init();
	virtual void apply();
	virtual void release();

	public slots:
		void open();
		void send();
protected:
     void closeEvent(QCloseEvent *event);
private:
	void loadSettings();
    void saveSettings();

	Ui::OpenFilePlugin ui;
	OpenFileStringSocket* _socket;
	//std::string _filename;
};

Etoile::EPlugin* loadOpenFilePlugin();

struct OpenFileDescription : public Etoile::EPluginsDescription
{
	OpenFileDescription()
	{
		this->menu = "Plugins";
		this->name = "OpenFile";
		this->plugins_load_functions.push_back(loadOpenFilePlugin);
		this->plugins_names.push_back("OpenFile");
	}
};

#endif // OPENFILEPLUGIN_H
