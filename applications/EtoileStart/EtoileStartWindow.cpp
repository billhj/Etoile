#include "EtoileStartWindow.h"

EtoileStartWindow::EtoileStartWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.toolBox->setItemText(0, "&Release");
	ui.toolBox->setItemText(1, "&Beta");
}

EtoileStartWindow::~EtoileStartWindow()
{

}
