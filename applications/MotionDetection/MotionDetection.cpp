#include "MotionDetection.h"

MotionDetection::MotionDetection(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	detector.apply();
}

MotionDetection::~MotionDetection()
{

}
