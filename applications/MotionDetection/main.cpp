#include "MotionDetection.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MotionDetection w;
	w.show();
	return a.exec();
}
