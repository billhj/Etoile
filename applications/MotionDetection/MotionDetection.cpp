#include "MotionDetection.h"
#include <QDir>

MotionDetection::MotionDetection(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	detector.setDirectory(QDir::currentPath().toStdString());
	vlayout = new QVBoxLayout(this);
	this->centralWidget()->setLayout(vlayout);
	button1 = new QPushButton("capture");
	button1->setCheckable(true);
    button2 = new QPushButton("show");
	button2->setCheckable(true);
	vlayout->addWidget(button1);
	vlayout->addWidget(button2);
	connect(button1, SIGNAL(toggled(bool)), this, SLOT(reactToToggleCapture(bool)));
	connect(button2, SIGNAL(toggled(bool)), this, SLOT(reactToToggleShow(bool)));
}

MotionDetection::~MotionDetection()
{

}

void MotionDetection::closeEvent(QCloseEvent *event)
{
	QMainWindow::closeEvent(event);
	exit(0);
}

void MotionDetection::reactToToggleCapture(bool checked)
 {
    if (checked) {
		detector.setEnable(true);
		t.setMotionDetector(&detector);
		t.start();
		button1->setText("pause capture");
	}else{
		detector.setEnable(false);
		button1->setText("capture");
	}
 }

void MotionDetection::reactToToggleShow(bool checked)
 {
    if (checked) {
		detector.setShow(true);
		button2->setText("unshow");
		
	}else{
		detector.setShow(false);
		button2->setText("show");
	}
 }