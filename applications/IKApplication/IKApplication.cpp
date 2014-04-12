#include "IKApplication.h"

IKApplication::IKApplication(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	_pIKWidget = new IKWidget();
	this->setCentralWidget(_pIKWidget);
	addMenu();
	this->setWindowTitle("IKViewer");

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(setMaxIterationsNb(int)));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDistanceThreshold(double)));
}

IKApplication::~IKApplication()
{

}

void IKApplication::addMenu()
{
	QMenuBar* bar = this->menuBar();
	QMenu* ik = bar->addMenu("IK");
	QAction* MS = ik->addAction("MassSpring");
	QAction* CCD = ik->addAction("CCD");
	QAction* PI = ik->addAction("JacobianPseudoInverse");
	QAction* TRS = ik->addAction("JacobianTranspose");
	QAction* DLS = ik->addAction("JacobianDLS");
	connect(ik, SIGNAL(triggered(QAction*)), this, SLOT(applyIKAction(QAction*)));

	QMenu* help = bar->addMenu("Help");
	QAction* usage = help->addAction("How to use");
	QAction* about = help->addAction("About IKViewer");
	connect(about, SIGNAL(triggered()), this, SLOT(openAbout()));
	connect(usage, SIGNAL(triggered()), this, SLOT(openUsage()));
	
}

void IKApplication::applyIKAction(QAction* action)
{
	QString t = action->text();
	if(t.toStdString() == "MassSpring")
	{
		_pIKWidget->setIKSolver(new Etoile::MassSpringSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "CCD")
	{
		_pIKWidget->setIKSolver(new Etoile::CCDSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianPseudoInverse")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianPseudoInverseSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianTranspose")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianTransposeSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianDLS")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianDLSSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	std::cout<<"now using : " <<t.toStdString()<<std::endl;
}

void IKApplication::openAbout()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: Jing HUANG \n jing.huang@telecom-paristech.fr    ");
	//msgBox.setInformativeText("Do you want to save your changes?");
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
}

void IKApplication::openUsage()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: Jing HUANG \n jing.huang@telecom-paristech.fr \n 1: Shift + Right_Mouse: select the joint \n 2: use the manipulator to apply IK");
	int ret = msgBox.exec();
}

void IKApplication::setMaxIterationsNb(int i)
{
	_pIKWidget->getIKSolver()->setMaxNumberOfTries(i);
}

void IKApplication::setDistanceThreshold(double d)
{
	_pIKWidget->getIKSolver()->setTargetThreshold(d);
}