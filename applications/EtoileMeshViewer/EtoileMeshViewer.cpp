/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileMeshViewer.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileMeshViewer.h"

EtoileMeshViewer::EtoileMeshViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	_pWidget = new MeshViewer();
	this->setCentralWidget(_pWidget);
	_pCameraWidget = new CameraWidget("Camera", _pWidget);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(2),_pCameraWidget);
	addMenu();
}


EtoileMeshViewer::~EtoileMeshViewer()
{

}

void EtoileMeshViewer::addMenu()
{
	QMenuBar* bar = this->menuBar();
    QMenu* file = bar->addMenu("File");
    QAction* open = file->addAction("open");
    connect(open, SIGNAL(triggered()), this, SLOT(openOBJ()));

	QMenu* renderer = bar->addMenu("Renderer");
    QAction* Basic = renderer->addAction("Basic");
	//QAction* BasicGpu = renderer->addAction("BasicGpu");
	//QAction* MultiPasses = renderer->addAction("MultiPasses");
	//QAction* OpenPassFile = renderer->addAction("OpenPassFile");
	//QAction* reloadPassFile = renderer->addAction("reloadPassFile");
    connect(Basic, SIGNAL(triggered()), _pWidget, SLOT(setBasicRenderer()));
	//connect(BasicGpu, SIGNAL(triggered()), _pWidget, SLOT(setBasicGPUProgramRenderer()));
	//connect(MultiPasses, SIGNAL(triggered()), _pWidget, SLOT(setGPUProgramRenderer()));
	//connect(OpenPassFile, SIGNAL(triggered()), _pWidget, SLOT(setPassFileRenderer()));
	//connect(reloadPassFile, SIGNAL(triggered()), _pWidget, SLOT(reloadPassRenderer()));

	QMenu* Gpu = bar->addMenu("GpuPro");
    QAction* load = Gpu->addAction("load");
	QAction* reload = Gpu->addAction("reload");
	connect(load, SIGNAL(triggered()), _pWidget, SLOT(loadGpuPro()));
	connect(reload, SIGNAL(triggered()), _pWidget, SLOT(reloadGpuPro()));

	QMenu* Light = bar->addMenu("Light");
    QAction* loadLight = Light->addAction("loadLight");
	QAction* reloadLight = Light->addAction("reloadLight");
	connect(loadLight, SIGNAL(triggered()), _pWidget, SLOT(loadLight()));
	connect(reloadLight, SIGNAL(triggered()), _pWidget, SLOT(reloadLight()));

	QMenu* help = bar->addMenu("Help");
    QAction* about = help->addAction("About OBJMeshViewer");
	connect(about, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void EtoileMeshViewer::openOBJ()
{
	_pWidget->readMeshFile();
}

void EtoileMeshViewer::openAbout()
{
	 QMessageBox msgBox;
	 msgBox.setText(" OBJMeshViewer \n Copyright(C) 2009-2012 \n author: Jing HUANG \n jing.huang@telecom-paristech.fr    ");
	 //msgBox.setInformativeText("Do you want to save your changes?");
	 //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	 //msgBox.setDefaultButton(QMessageBox::Save);
	 int ret = msgBox.exec();

}

void EtoileMeshViewer::setMesh(Etoile::Mesh* signal)
{
	if(signal == NULL) return;
	_pWidget->setMesh(signal);
}

void EtoileMeshViewer::openFile(const std::string& signal)
{
	_pWidget->open(signal);
}