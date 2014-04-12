#include "GridFluidWindow.h"

GridFluidWindow::GridFluidWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_widget = new FluidWidget();
	this->setCentralWidget(_widget);
}

GridFluidWindow::~GridFluidWindow()
{

}
