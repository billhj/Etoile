#include "EtoileStartWindow.h"
#include <QSettings>
#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QWindowsStyle>
#include <QThread>
#include <windows.h>
EtoileStartWindow::EtoileStartWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	_buttongroup = new QButtonGroup();
	_buttongroup->setExclusive(false);
	ui.toolBox->setItemText(0, "&Release");
	ui.toolBox->setItemText(1, "&Beta");

	_initfile = "etoile.ini";
	loadInit();
	loadInterface();
	connect(_buttongroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
	//QWindowsStyle, QMacStyle, QMotifStyle, "Plastic"
	//"windows", "motif", "cde", "plastique", "windowsxp", or "macintosh"
	QApplication::setStyle("windows");
}

EtoileStartWindow::~EtoileStartWindow()
{

}


void EtoileStartWindow::loadInit()
{
	QSettings settings(_initfile, QSettings::IniFormat);
	QString apps = settings.value("apps").toString();	

	QSettings settingapps(apps, QSettings::IniFormat);
	//debug//QString oob = settingapps.value("oob").toString();


	/*settingapps.beginWriteArray("apps");
    settingapps.setArrayIndex(0);
    settingapps.setValue("group", "1");
    settingapps.setValue("name", "22");
	settingapps.endArray();*/

	int groupIndex = 0;
	int size = settingapps.beginReadArray("app");
	for (int i = 0; i < size; ++i) 
	{
		settingapps.setArrayIndex(i);
		EApplicationHeader header;
		header._group = settingapps.value("group").toString();
		if(!_groupMap.contains(header._group))
		{
			_groupMap[header._group] = groupIndex;
			++groupIndex;
		}

		header._name = settingapps.value("name").toString();
		header._author = settingapps.value("author").toString();
#if defined(_DEBUG) || defined(DEBUG)	
		header._dllName = settingapps.value("dll_debug").toString();
#else if
		header._dllName = settingapps.value("dll").toString();
#endif
		header._loadfunction = settingapps.value("loadfunction").toString();
		header._unloadfunction = settingapps.value("unloadfunction").toString();
		_appHeaders.append(header);
	}
	settingapps.endArray();
}

void EtoileStartWindow::loadInterface()
{
	int count = ui.toolBox->count();
	foreach(QString group, _groupMap.keys())
	{
		int index = _groupMap[group];
		if(index >= count)
		{
			ui.toolBox->insertItem(index, new QWidget(), "&"+group);
			++count;
		}
		else
		{
			ui.toolBox->setItemText(index, "&"+group);
		}
		QWidget* widget = ui.toolBox->widget(index);
		QVBoxLayout *layout = new QVBoxLayout;
		widget->setLayout(layout);
		layout->setAlignment(Qt::AlignTop);
	}

	foreach(EApplicationHeader header, _appHeaders)
	{
		QString group = header._group;
		QWidget* widget = ui.toolBox->widget(_groupMap[group]);
		QPushButton* button = new QPushButton(header._name);
		button->setCheckable(true);
		//button->setAutoExclusive(false);
		button->setFlat(false);
		button->setStyleSheet("QPushButton { border: 2px solid rgb(200, 0, 180, 50); border-radius: 4px; background-color: rgb(100, 0, 100, 50);} QPushButton:checked { border: 3px solid rgb(0, 60, 50, 50); border-radius: 4px; background-color: rgb(100, 0, 100, 50);} QPushButton:hover { background-color: blue; } QPushButton:pressed{ border: 2px solid rgb(100, 100, 100, 50); padding-left : 2px;padding-top : 2px;border-radius: 4px; }");
		QGraphicsDropShadowEffect *effet = new QGraphicsDropShadowEffect(widget);
		effet->setColor(QColor(215, 195, 120, 255));
		effet->setOffset(2);
		effet->setBlurRadius(30);
		button->setGraphicsEffect(effet);
		//button->setStyleSheet("selection-background-color: rgb(143, 241, 255);alternate-background-color: rgb(71, 237, 255); background-color: grey; border-style: outset; border-width: 2px; border-radius: 5px; border-color: grey; font: bold 14px;");
		(widget->layout())->addWidget(button);
		_buttongroup->addButton(button);
	}
}

void EtoileStartWindow::buttonClicked(QAbstractButton * button)
{
	QString name = button->text();
	bool hasApp = false;
	for(unsigned int i = 0; i < _appHeaders.size(); ++i)
	{
		EApplicationHeader& header = _appHeaders[i];
		if(header._name.compare(name) == 0)
		{
			QString feedback;

			if(!button->isChecked())
			{
				/*if(_libs[header._dllName]->isLoaded())
				{*/
					releaseApp(header);
				/*}
				else
				{
					button->setChecked(true);
				}*/
			}
			else if(button->isChecked())
			{
				hasApp = callApp(header, feedback);

				if(!hasApp)
				{
					QMessageBox msgBox;
					msgBox.setText("cannot open EApp: " + header._name);
					msgBox.setDetailedText(header.detail().append(feedback));
					msgBox.exec();
				}
			}

			break;
		}
	}
	
}

typedef void (*StartApp)();
typedef void (*EndApp)();
bool EtoileStartWindow::callApp(EApplicationHeader& header, QString& feedback)
{
	bool callDll = false;
	QLibrary* mylib = new QLibrary(header._dllName);
	if (mylib->load())  
	{
		_libs[header._dllName] = mylib;
		StartApp function = (StartApp)mylib->resolve(header._loadfunction.toStdString().c_str());
		if(function)
		{
			function();
			callDll = true;
			feedback.append("function is loaded!");
			//mylib->unload();
			//Sleep(2000);
		}
		else
		{
			callDll = false;
			feedback.append("function is not found!");
		}
	}
	else
	{
		callDll = false;
		feedback.append("dll is not loaded!");
	}

	return callDll;
}

void EtoileStartWindow::releaseApp(EApplicationHeader& header)
{
	EndApp function = (EndApp)_libs[header._dllName]->resolve(header._unloadfunction.toStdString().c_str());
	if(function)
	{
		function();
		bool unload = _libs[header._dllName]->unload();
		_libs.remove(header._dllName);
	}
}

void EtoileStartWindow::showAbout()
{
	QMessageBox msgBox;
	QString about = "author: Jing HUANG \n";
	about.append("email: gabriel.jing.huang@gmail.com \n");
	about.append("Copyright (C) <2012> <Jing HUANG>");
	msgBox.setText(about);
	msgBox.exec();
}