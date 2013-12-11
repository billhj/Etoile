#include "OpenFilePlugin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

Etoile::EPlugin* loadOpenFilePlugin()
{
	return new OpenFilePlugin("OpenFile");
}

OpenFilePlugin::OpenFilePlugin(const std::string name, QWidget *parent):Etoile::EPlugin(), QDockWidget(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	this->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	//this->setFloating(false);
	this->getType()._description = "OpenFile";
	this->getType()._name = name;
	this->getType()._w = 80;
	this->getType()._h = 40;
	this->getType()._color._r = 100;
	this->getType()._color._g = 100;
	this->getType()._color._b = 100;
	this->getType()._color._a = 255;
	connect(ui.open, SIGNAL(clicked()), this, SLOT(open()));
	connect(ui.send, SIGNAL(clicked()), this, SLOT(send()));

	this->setVisible(false);
	_socket = new OpenFileStringSocket("FileName");
	_socket->initType("SOCK_STRING");
	this->addOutputSocket(_socket);
	loadSettings();
}

OpenFilePlugin::~OpenFilePlugin()
{

}

void OpenFilePlugin::init()
{
	
}

void OpenFilePlugin::apply()
{
	this->setVisible(true);
	this->raise();
}

void OpenFilePlugin::release()
{
}

void OpenFilePlugin::open()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		ui.filename->setText(fileName);
		_socket->signalEmit(&fileName.toStdString());
	}else{
		QMessageBox::information(this, tr("OpenFile"),
			tr("Cannot load %1.").arg(fileName));
	}
	
}

void OpenFilePlugin::send()
{
	QString file = ui.filename->text();
	if(!file.isEmpty())
	{
		_socket->signalEmit(&file.toStdString());
	}
}


void OpenFilePlugin::loadSettings()
{
    QSettings settings("OpenFilePlugin.ini", QSettings::IniFormat);
	ui.filename->setText(settings.value("path").toString());	
}
 
void OpenFilePlugin::saveSettings()
{
    QSettings settings("OpenFilePlugin.ini", QSettings::IniFormat);
    settings.setValue("path", ui.filename->text());
	settings.sync();
}

void OpenFilePlugin::closeEvent(QCloseEvent *event)
{
	 saveSettings();
}