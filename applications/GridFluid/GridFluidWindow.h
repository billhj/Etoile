#ifndef GridFluidWindow_H
#define GridFluidWindow_H

#include <QtGui/QMainWindow>
#include "ui_GridFluidWindow.h"
#include "FluidWidget.h"

class GridFluidWindow : public QMainWindow
{
	Q_OBJECT

public:
	GridFluidWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GridFluidWindow();

private:
	Ui::GridFluidWindowClass ui;
	FluidWidget* _widget;
};

#endif // GridFluidWindow_H
