#include "EtoileStartWindow.h"
#include <QSettings>
#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QLibrary>

EtoileStartWindow::EtoileStartWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_buttongroup = new QButtonGroup();
	ui.toolBox->setItemText(0, "&Release");
	ui.toolBox->setItemText(1, "&Beta");

	_initfile = "etoile.ini";
	loadInit();
	loadInterface();
	connect(_buttongroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

	QApplication::setStyle("Plastic");
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
		header._functionName = settingapps.value("function").toString();

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
		button->setFlat(false);
		button->setStyleSheet("QPushButton { border: 2px solid rgb(100, 0, 100, 50); border-radius: 4px; background-color: rgb(100, 0, 100, 50);} QPushButton:hover { background-color: blue; } QPushButton:pressed{ border: 2px solid rgb(100, 100, 100, 50); padding-left : 2px;padding-top : 2px;border-radius: 4px; }");
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
	foreach(EApplicationHeader header, _appHeaders)
	{
		if(header._name.compare(name) == 0)
		{
			QString feedback;
			hasApp = callApp(header, feedback);

			if(!hasApp)
			{
				QMessageBox msgBox;
				msgBox.setText("cannot open EApp: " + header._name);
				msgBox.setDetailedText(header.detail().append(feedback));
				msgBox.exec();
			}

			break;
		}
	}
	
}

typedef void (*StartApp)();

bool EtoileStartWindow::callApp(EApplicationHeader header, QString& feedback)
{
	bool callDll = false;
	QLibrary mylib(header._dllName);
	if (mylib.load())  
	{
		StartApp function = (StartApp)mylib.resolve(header._functionName.toStdString().c_str());
		if(function)
		{
			function();
			callDll = true;
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