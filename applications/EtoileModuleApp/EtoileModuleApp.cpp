#include "EtoileModuleApp.h"
//#include "ImageViewer\ImageViewerPlugin.h"
#include <QMessageBox>
#include <QPalette>
#include <QLibrary> 
#include "OpenFilePlugin.h"
#include <QSettings>
#include <QFileDialog>
#include "tinyxml/tinyxml.h"
#include <QTimer>

EtoileModuleApp::EtoileModuleApp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->setWindowTitle("Etoile");
	_editor = new GraphEditor(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	this->setCentralWidget(_editor);
	_editor->setVisible(false);
	initPlugins();
	createMenus();
	loadStyle();
	_pPalette = new QPalette();	
	_pPixmap = new QPixmap("img/bg.png");
	_pPalette->setBrush(QPalette::Foreground,*(new QBrush(*(_pPixmap))));
 	setPalette(*_pPalette); 	
}

void EtoileModuleApp::resizeEvent(QResizeEvent* event)
{
      _pPalette->setBrush(QPalette::Background,QBrush(_pPixmap->scaled(width(),height())));
      setPalette(*_pPalette);
};

EtoileModuleApp::~EtoileModuleApp()
{
	
}

//extern "C" void loadPlugin(Etoile::EPlugin* plug);

void EtoileModuleApp::initPlugins()
{
	/*QLibrary mylib("ImageViewerd.dll");
	if (mylib.load())  
	{
		Fun load = (Fun)mylib.resolve("loadPlugin");
		Etoile::EPlugin* plugin = NULL;
		load(&plugin);
		Etoile::EPluginManager* pluginmanager = _core.getPluginManager();
		pluginmanager->add(plugin);
	}*/
	//Etoile::ImageViewerPlugin* plugin = new Etoile::ImageViewerPlugin("ImageViewer");
	_core.loadPlugins();
	Etoile::EPluginManager* pluginmanager = _core.getPluginManager();
	OpenFileDescription* openfile = new OpenFileDescription();
	pluginmanager->add(openfile);
}


QMenu* EtoileModuleApp::getMenuByName(QString name)
{
	for(unsigned int i = 0; i < _myMenuList.size(); ++i)
	{
		QMenu* menu = _myMenuList[i];
		if(menu->title().compare(name) == 0)
		{
			return menu;
		}
	}

	return NULL;
}

void EtoileModuleApp::changeStyle()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("select StyleSheet"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		_stylefile = fileName;
		QFile file(_stylefile);
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		qApp->setStyleSheet(styleSheet);
	}
	else
	{
		QMessageBox::information(this, tr("OpenFile"),
			tr("Cannot load %1.").arg(fileName));
	}
}

void EtoileModuleApp::loadStyle()
{
	QSettings settings("style.ini", QSettings::IniFormat);
	_stylefile = settings.value("stylepath").toString();	
	_graph = settings.value("graphpath").toString();	
	loadGraph(_graph);
	QFile file(_stylefile);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}

void EtoileModuleApp::saveStyle()
{
	QSettings settings("style.ini", QSettings::IniFormat);
    settings.setValue("stylepath", _stylefile);
	settings.setValue("graphpath", _graph);
	settings.sync();
}

void EtoileModuleApp::createMenus()
{
	QAction* exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	QAction* changeStyle = new QAction(tr("&changeStyle"), this);
	connect(changeStyle, SIGNAL(triggered()), this, SLOT(changeStyle()));

    fileMenu = new QMenu("File", this);
    fileMenu->addSeparator();
	fileMenu->addAction(changeStyle);
    fileMenu->addAction(exitAct);
	

    pluginMenu = new QMenu("Plugins", this);
	QAction* graphAct = new QAction("GraphEditor", this);
	QAction* loadgraph = new QAction("loadGraph", this);
	QAction* savegraph = new QAction("saveGraph", this);
	connect(loadgraph, SIGNAL(triggered()), this, SLOT(loadGraph()));
	connect(savegraph, SIGNAL(triggered()), this, SLOT(saveGraph()));
	QTimer::singleShot(2000, graphAct, SLOT(trigger()));
	graphAct->setShortcut(tr("Ctrl+G"));
	pluginMenu->addAction(graphAct);
	pluginMenu->addAction(loadgraph);
	pluginMenu->addAction(savegraph);
	pluginMenu->addSeparator();

    helpMenu = new QMenu(tr("&Help"), this);
	QAction* aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAct);
	
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(pluginMenu);
	menuBar()->addMenu(helpMenu);
	_myMenuList.push_back(fileMenu);
	_myMenuList.push_back(pluginMenu);
	_myMenuList.push_back(helpMenu);
	//menuBar()->
//PLUGINS
	//OpenFilePlugin* openfile = new OpenFilePlugin("OpenFile", this);
	//QAction* openfile = new QAction(tr("OpenFile"), this);
	//pluginMenu->addAction(openfile);
	Etoile::EPluginManager* pluginmanager = _core.getPluginManager();
	std::vector<Etoile::EPluginsDescription*>& descriptions = pluginmanager->getDescriptions();
	std::vector<Etoile::EPluginsDescription*>::iterator itor = descriptions.begin();
	for(; itor != descriptions.end(); ++itor)
	{
		Etoile::EPluginsDescription* desc = *itor;
		QMenu* menu = getMenuByName(QString(desc->menu.c_str()));//->addAction(plug);
		if(menu == NULL)
		{
			menu = new QMenu(QString(desc->menu.c_str()), this);
			menuBar()->addMenu(menu);
			_myMenuList.push_back(menu);
		}

		unsigned int nb_plugins = desc->plugins_names.size();
		if(nb_plugins > 0)
		{
			if(nb_plugins == 1 && desc->name.compare(desc->plugins_names[0]) == 0)
			{
				QAction* plug = new QAction(tr(desc->plugins_names[0].c_str()), menu);
				menu->addAction(plug);
			}
			else
			{
				QMenu* secondmenu = getMenuByName(QString(desc->name.c_str()));
				if(secondmenu == NULL)
				{
					secondmenu = new QMenu(QString(desc->name.c_str()), menu);
					menu->addMenu(secondmenu);
					_myMenuList.push_back(secondmenu);
				}

				for(unsigned int i = 0; i < nb_plugins; ++i)
				{
					QAction* plug = new QAction(tr(desc->plugins_names[i].c_str()), menu);
					secondmenu->addAction(plug);
				}
			}
		}
	}
	connect(pluginMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyAction(QAction *)));

}

void EtoileModuleApp::applyAction(QAction * action)
{
	if(action->text().toStdString().compare("GraphEditor")==0)
	{
		_editor->show();
	}
	else
	{
		Etoile::EPluginManager* pluginmanager = _core.getPluginManager();
		std::vector<Etoile::EPluginsDescription*> descs = pluginmanager->getDescriptions();
		for(unsigned int j = 0; j < descs.size(); ++j)
		{
			Etoile::EPluginsDescription* desc = descs[j];
			if(desc != NULL)
			{
				for(unsigned int i = 0; i < desc->plugins_names.size(); ++i)
				{
					if(desc->plugins_names[i].compare(action->text().toStdString())==0)
					{
						Etoile::EPlugin* plugin = desc->plugins_load_functions[i]();
						plugin->init();
						Etoile::SocketNode * snode = dynamic_cast<Etoile::SocketNode*>(plugin);
						if(snode != NULL)
						{
							_editor->addNode(snode);
						}
						
					}
				}
			}
		}
	}
}

void EtoileModuleApp::loadGraph(QString fileName)
{
	if (!fileName.isEmpty()) {
		TiXmlDocument doc(fileName.toStdString().c_str());
		if(!doc.LoadFile())
		{
			std::cout << "erreur while loading: " << fileName.toStdString()<<std::endl;
			std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
			QMessageBox::information(this, tr("OpenFile"),
			tr("Cannot load %1.").arg(fileName));
		}
		TiXmlHandle hdl(&doc);

		TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
		while (elemRoot)
		{
			TiXmlElement *nodeElement = elemRoot->FirstChildElement("Node");
			while (nodeElement)
			{
				std::string name = nodeElement->Attribute("name");
				int x;
				if(TIXML_NO_ATTRIBUTE==nodeElement->QueryIntAttribute("x", &x))
				{
					x = 0;
				}
				int y;
				if(TIXML_NO_ATTRIBUTE==nodeElement->QueryIntAttribute("y", &y))
				{
					y = 0;
				}

				Etoile::EPluginManager* pluginmanager = _core.getPluginManager();
				std::vector<Etoile::EPluginsDescription*> descs = pluginmanager->getDescriptions();
				for(unsigned int j = 0; j < descs.size(); ++j)
				{
					Etoile::EPluginsDescription* desc = descs[j];
					if(desc != NULL)
					{
						for(unsigned int i = 0; i < desc->plugins_names.size(); ++i)
						{
							if(desc->plugins_names[i].compare(name)==0)
							{
								Etoile::EPlugin* plugin = desc->plugins_load_functions[i]();
								plugin->init();
								Etoile::SocketNode * snode = dynamic_cast<Etoile::SocketNode*>(plugin);
								snode->getType()._xPos = x;
								snode->getType()._yPos = y;
								if(snode != NULL)
								{
									_editor->addNode(snode);
								}	
							}
						}
					}
				}
				nodeElement = nodeElement->NextSiblingElement("Node"); // iteration
			}


			TiXmlElement *connectionElement = elemRoot->FirstChildElement("Connection");
			while (connectionElement)
			{
				int fromNode;
				if(TIXML_NO_ATTRIBUTE==connectionElement->QueryIntAttribute("fromNode", &fromNode))
				{
					fromNode = 0;
				}
				int toNode;
				if(TIXML_NO_ATTRIBUTE==connectionElement->QueryIntAttribute("toNode", &toNode))
				{
					toNode = 0;
				}
				int fromSocket;
				if(TIXML_NO_ATTRIBUTE==connectionElement->QueryIntAttribute("fromSocket", &fromSocket))
				{
					fromSocket = 0;
				}
				int toSocket;
				if(TIXML_NO_ATTRIBUTE==connectionElement->QueryIntAttribute("toSocket", &toSocket))
				{
					toSocket = 0;
				}

				Etoile::GraphScene* graph = _editor->getGraph();
				if(graph != NULL)
				{
					std::vector<Etoile::BasicNode*>& nodes = graph->getNodes();
					Etoile::OutputSocket* os = NULL;
					Etoile::InputSocket* is = NULL;
					if(fromNode < nodes.size())
					{
						Etoile::SocketNode* snode = dynamic_cast<Etoile::SocketNode*>(nodes[fromNode]);
						if(snode != NULL)
						{
							if(snode->getOutputSockets().size() > fromSocket)
							{
								os = snode->getOutputSockets()[fromSocket];
							}
						}
					}
					if(toNode < nodes.size())
					{
						Etoile::SocketNode* snode = dynamic_cast<Etoile::SocketNode*>(nodes[toNode]);
						if(snode != NULL)
						{
							if(snode->getInputSockets().size() > toSocket)
							{
								is = snode->getInputSockets()[toSocket];
							}
						}
					}
					_editor->addConnection(os, is);
				}

				connectionElement = connectionElement->NextSiblingElement("Connection"); // iteration
			}

			elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
		}

		
	}
	else
	{
		QMessageBox::information(this, tr("OpenFile"),
			tr("Cannot load %1.").arg(fileName));
	}
}

void EtoileModuleApp::saveGraph()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save GraphFile", QDir::currentPath(), "*.xml");
	TiXmlDocument doc;
	if(_editor != NULL)
	{
		Etoile::GraphScene* graph = _editor->getGraph();
		if(graph != NULL)
		{
			TiXmlElement * root = new TiXmlElement( "Etoile" );
			doc.LinkEndChild(root);
			std::vector<Etoile::BasicNode*>& nodes = graph->getNodes();
			for(unsigned int i = 0; i < nodes.size(); ++i)
			{
				Etoile::BasicNode* node = nodes[i];
				Etoile::SocketNode* snode = dynamic_cast<Etoile::SocketNode*>(node);
				if(snode != NULL)
				{
					TiXmlElement * nodeElement = new TiXmlElement( "Node" );
					nodeElement->SetAttribute("name", node->getName().c_str());
					nodeElement->SetAttribute("x", node->getType()._xPos);
					nodeElement->SetAttribute("y", node->getType()._yPos);
					root->LinkEndChild(nodeElement);
				}
				else if(node != NULL)
				{
					TiXmlElement * nodeElement = new TiXmlElement( "Node" );
					nodeElement->SetAttribute("name", node->getName().c_str());
					nodeElement->SetAttribute("x", node->getType()._xPos);
					nodeElement->SetAttribute("y", node->getType()._yPos);
					root->LinkEndChild(nodeElement);
				}
			}

			std::vector<Etoile::SocketConnection*>& connections = graph->getSocketConnections();
			for(unsigned int i = 0; i < connections.size(); ++i)
			{
				Etoile::SocketConnection* conn = connections[i];
				Etoile::SocketNode* nodeF = conn->getFromNode();
				Etoile::SocketNode* nodeT = conn->getToNode();
				Etoile::OutputSocket* socketF = conn->getFromSocket();
				Etoile::InputSocket* socketT = conn->getToSocket();
				TiXmlElement * connElement = new TiXmlElement( "Connection" );
				connElement->SetAttribute("fromNode", graph->getNodeIndex(nodeF));
				connElement->SetAttribute("toNode", graph->getNodeIndex(nodeT));
				connElement->SetAttribute("fromSocket", nodeF->getOutputSocketIndex(socketF));
				connElement->SetAttribute("toSocket", nodeT->getInputSocketIndex(socketT));
				root->LinkEndChild(connElement);
			}

			doc.SaveFile(fileName.toStdString().c_str());
		}
	}
}
void EtoileModuleApp::loadGraph()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("select GraphFile"), QDir::currentPath());
	loadGraph(fileName);
	_graph = fileName;
}

void EtoileModuleApp::about()
{
    QMessageBox::about(this, tr("About Etoile"),
                       tr("Plugin System made by Dr. Jing HUANG"));
}

void EtoileModuleApp::closeEvent(QCloseEvent *event)
{
	saveStyle();
	qApp->quit();
}