#include "etoilestartwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EtoileStartWindow w;
	w.show();
	return a.exec();
}
