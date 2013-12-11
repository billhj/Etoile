/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MocapViewer.h
* @brief 
* @date 1/2/2011
*/

#include "MocapViewer.h"
#include <QMessageBox>

MocapViewer::MocapViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	_pMocapWidget = new MocapWidget(this);
	this->setCentralWidget(_pMocapWidget);
	ui.dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

	checkPauseButton();
	checkCycleButton();
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushPauseButton()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(pushCycleButton()));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slideFrame(int)));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
	connect(_pMocapWidget, SIGNAL(paused()), this, SLOT(checkPauseButton()));
	connect(_pMocapWidget, SIGNAL(frameChanged(int)), ui.horizontalSlider, SLOT(setValue(int)));
	
	ui.horizontalSlider->setSingleStep(1);
	ui.spinBox->setRange(1,1500);
	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinChangeFps(int)));
	ui.lineEdit->setEnabled(false);
	addMenu();

}

MocapViewer::~MocapViewer()
{

}

void MocapViewer::addMenu()
{
	QMenuBar* bar = this->menuBar();
    QMenu* file = bar->addMenu("File");
    QAction* open = file->addAction("open");
    connect(open, SIGNAL(triggered()), this, SLOT(openBVH()));

	QMenu* help = bar->addMenu("Help");
    QAction* about = help->addAction("About MocapViewer");
	connect(about, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void MocapViewer::openFile(const std::string& signal)
{
	_pMocapWidget->initFramework(signal);
}

void MocapViewer::openBVH()
{
	_pMocapWidget->openBVH();
	checkPauseButton();
	checkCycleButton();
	unsigned int size = _pMocapWidget->_bvhloader.getKeyFrameSequence()->getKeyFrames().size();
	ui.horizontalSlider->setRange(0, size-1);
	ui.horizontalSlider->setTickPosition(QSlider::TicksBothSides);
	if(size > 1)
	{
		float time = _pMocapWidget->_bvhloader.getKeyFrameSequence()->getKeyFrames()[1]->getTime() - _pMocapWidget->_bvhloader.getKeyFrameSequence()->getKeyFrames()[0]->getTime();
		//spinChangeFps(1000.0 / time);
		ui.spinBox->setValue(1.0 / time);
	}
}

void MocapViewer::openAbout()
{
	 QMessageBox msgBox;
	 msgBox.setText(" MocapViewer \n Copyright(C) 2009-2012 \n author: Jing HUANG \n jing.huang@telecom-paristech.fr    ");
	 //msgBox.setInformativeText("Do you want to save your changes?");
	 //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	 //msgBox.setDefaultButton(QMessageBox::Save);
	 int ret = msgBox.exec();

}

void MocapViewer::checkPauseButton()
{
	if(_pMocapWidget->_pause)
	{
		ui.pushButton->setText("Start");
	}
	else
	{
		ui.pushButton->setText("Pause");
	}
}

void MocapViewer::pushPauseButton()
{
	_pMocapWidget->_pause = !_pMocapWidget->_pause;
	checkPauseButton();
}

void MocapViewer::checkCycleButton()
{
	if(_pMocapWidget->_cycle)
	{
		ui.pushButton_2->setText("No Cycle");
	}
	else
	{
		ui.pushButton_2->setText("Cycle");	
	}
}

void MocapViewer::pushCycleButton()
{
	_pMocapWidget->_cycle = !_pMocapWidget->_cycle;
	checkCycleButton();
}

void MocapViewer::slideFrame(int value)
{
	_pMocapWidget->updateFrame(value);
}

void MocapViewer::sliderChanged(int value)
{
	int max = ui.horizontalSlider->maximum();
	ui.lineEdit->setText(QString(" frame : [ %1 / %2] ").arg(value).arg(max));
}

void MocapViewer::spinChangeFps(int value)
{
	_pMocapWidget->setFPS(value);
}

void MocapViewer::closeEvent(QCloseEvent *event)
{
     this->setCentralWidget(NULL);
	 //delete _pMocapWidget;
}