#include "EtoileGraphEditor.h"
#include <QtGui/QApplication>
#include "object/EPlugin.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EtoileGraphEditor w;
	w.show();
	return a.exec();
}
