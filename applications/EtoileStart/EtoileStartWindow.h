#ifndef ETOILESTARTWINDOW_H
#define ETOILESTARTWINDOW_H

#include <QMainWindow>
#include "ui_EtoileStartWindow.h"


struct EApplicationHeader 
{
	QString _group;
	QString _name;
	QString _author;
	QString _dllName;
	QString _functionName;

	QString detail()
	{
		QString detail;
		detail.append("group: "+_group+" \n");
		detail.append("name: "+_name+" \n");
		detail.append("author: "+_author+" \n");
		detail.append("dll: "+_dllName+" \n");
		detail.append("function: "+_functionName+" \n");
		return detail;
	}
};


class EtoileStartWindow : public QMainWindow
{
	Q_OBJECT

public:
	EtoileStartWindow(QWidget *parent = 0);
	~EtoileStartWindow();
	void loadInit();
	void loadInterface();
	bool callApp(EApplicationHeader header);
	public slots:
		void buttonClicked(QAbstractButton * button);

private:
	Ui::EtoileStartWindow ui;
	QString _initfile;
	QList<EApplicationHeader> _appHeaders;
	QMap<QString, int> _groupMap;
	QButtonGroup* _buttongroup;
};

#endif // ETOILESTARTWINDOW_H
