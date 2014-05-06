#include "DynamicsSkeletonDemo.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DynamicsSkeletonDemo w;
	w.show();
	return a.exec();
}
