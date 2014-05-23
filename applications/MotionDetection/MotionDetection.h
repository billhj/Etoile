#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H

#include <QtGui/QMainWindow>
#include "ui_MotionDetection.h"
#include "MotionDetector.h"

class MotionDetection : public QMainWindow
{
	Q_OBJECT

public:
	MotionDetection(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MotionDetection();

private:
	Ui::MotionDetectionClass ui;
	Etoile::MotionDetector detector;
};

#endif // MOTIONDETECTION_H
