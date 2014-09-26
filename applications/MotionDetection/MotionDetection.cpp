#include "MotionDetection.h"
#include <QDir>
MotionDetection::MotionDetection(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	detector.setDirectory(QDir::currentPath().toStdString());
	detector.apply();
	this->show();
}

MotionDetection::~MotionDetection()
{

}

void MotionDetection::start()
{
	detector.apply();
}

void MotionDetection::closeEvent(QCloseEvent *event)
{
	QMainWindow::closeEvent(event);
	exit(0);
}