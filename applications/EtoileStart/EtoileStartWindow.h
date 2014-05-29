#ifndef ETOILESTARTWINDOW_H
#define ETOILESTARTWINDOW_H

#include <QMainWindow>
#include "ui_EtoileStartWindow.h"

class EtoileStartWindow : public QMainWindow
{
	Q_OBJECT

public:
	EtoileStartWindow(QWidget *parent = 0);
	~EtoileStartWindow();

private:
	Ui::EtoileStartWindow ui;
};

#endif // ETOILESTARTWINDOW_H
