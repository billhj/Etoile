#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H

#include <QtGui/QMainWindow>
#include "ui_MotionDetection.h"
#include "MotionDetector.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <qthread.h>


class WorkerThread : public QThread
{
	Q_OBJECT
	
public:
	Etoile::MotionDetector* _detector;
	void setMotionDetector(Etoile::MotionDetector* detector)
	{
		_detector = detector;
	}


	void run()  {
			_detector->apply();
	}

};

class MotionDetection : public QMainWindow
{
	Q_OBJECT

public:
	MotionDetection(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MotionDetection();
	void closeEvent(QCloseEvent *event);
public slots:
	void reactToToggleCapture(bool checked);
	void reactToToggleShow(bool checked);
private:
	Ui::MotionDetectionClass ui;
	Etoile::MotionDetector detector;
	QVBoxLayout* vlayout;
	QPushButton *button1;
	QPushButton *button2;
	WorkerThread t;
};



#endif // MOTIONDETECTION_H
