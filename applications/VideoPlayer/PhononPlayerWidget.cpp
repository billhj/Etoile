#include "PhononPlayerWidget.h"
#include <QFileDialog>
#include <qicon.h>
#include <phonon/mediaobject>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>
#include "PlayListWidget.h"

PhononPlayerWidget::PhononPlayerWidget(Phonon::Category category, QWidget * parent) : Phonon::VideoPlayer(category, parent), _pPlaylist(NULL)
{
	setWindowIcon(QIcon("./styles/icon.png"));
	setWindowTitle("EtoileVideoPlayer");
	setGeometry(200, 200, 800, 600);
	//resize(800, 600);
	this->setStyleSheet("background-image: url(styles/bg.png)");
	//connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	QPalette* _pPalette;
	QPixmap* _pPixmap;
	_pPalette = new QPalette();	
	_pPixmap = new QPixmap("img/bg.png");
	_pPalette->setBrush(QPalette::Foreground,*(new QBrush(*(_pPixmap))));
	setPalette(*_pPalette); 	
	this->mediaObject()->setTickInterval(1000);
	setAcceptDrops(true);
}

PhononPlayerWidget::~PhononPlayerWidget()
{

}

void PhononPlayerWidget::bindPlayList(PlayListWidget* playlist)
{
	_pPlaylist = playlist;
}


void PhononPlayerWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	if(this->windowState() == Qt::WindowFullScreen)
	{
		setGeometry(200, 200, 800, 600);
		//resize(800, 600);
	}else
	{
		this->setWindowState(Qt::WindowFullScreen);
	}
}

void PhononPlayerWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void PhononPlayerWidget::dropEvent(QDropEvent * event)
{
	if(_pPlaylist == NULL) return;
	const QMimeData* mimeData = event->mimeData();

	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		_pPlaylist->addSources(urlList);
		Phonon::VideoPlayer::play();
	}
}