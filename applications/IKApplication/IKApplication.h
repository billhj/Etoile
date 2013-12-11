#ifndef IKAPPLICATION_H
#define IKAPPLICATION_H

#include <QtGui/QMainWindow>
#include "ui_IKApplication.h"
#include "IKWidget.h"

class IKApplication : public QMainWindow
{
	Q_OBJECT

public:
	IKApplication(QWidget *parent = 0, Qt::WFlags flags = 0);
	~IKApplication();
	void addMenu();
public slots:
	void applyIKAction(QAction*);
	void openAbout();
	void openUsage();
	void setMaxIterationsNb(int);
	void setDistanceThreshold(double);
private:
	Ui::IKApplicationClass ui;
	IKWidget* _pIKWidget;
};

#endif // IKAPPLICATION_H
