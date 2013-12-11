#include "EtoileVideoPlayer.h"
#include <QFileDialog>
#include <QTime>
#include <phonon/mediaobject>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>
#include <qlayout.h>

EtoileVideoPlayer::EtoileVideoPlayer(QWidget *parent): QWidget(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	setWindowTitle("EtoileVideoPlayer");
	
	connect(ui.open, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(ui.play, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
	ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	ui.stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
	_pPlayer = new PhononPlayerWidget();
	ui.playlist->setPlayer(_pPlayer);
	ui.volumeSlider->setAudioOutput(_pPlayer->audioOutput());
	ui.seekSlider->setMediaObject(_pPlayer->mediaObject());
	ui.tabWidget->setTabText(0, "Player");
	ui.tabWidget->setTabText(1, "PlayList");
	ui.tabWidget->setCurrentIndex(0);

	ui.playtime->display("00:00");
	ui.play->setEnabled(false);
	connect((_pPlayer->mediaObject()), SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
	connect((_pPlayer), SIGNAL(finished()), this, SLOT(stop()));
	connect(ui.playlist, SIGNAL(chooseplay()), this, SLOT(play()));
	setAcceptDrops(true);
}

EtoileVideoPlayer::~EtoileVideoPlayer()
{

}

void EtoileVideoPlayer::openFile(const std::string& name)
{
	if (!name.empty()) {
		ui.play->setEnabled(true);
		ui.playlist->addSource(QString(name.c_str()));
		ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
}

void EtoileVideoPlayer::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),
		QDir::homePath(), "");

	if (!fileName.isEmpty()) {
		ui.play->setEnabled(true);
		ui.playlist->addSource(fileName);
		ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
}

void EtoileVideoPlayer::play()
{
	if(_pPlayer->isPlaying())
	{
		pause();
	}
	else
	{
		_pPlayer->play();
		_pPlayer->raise();
		_pPlayer->setVisible(true);
		ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		if(!_currentMedia.isEmpty())
		{
			_currentMedia = "";
		}
	}
}

void EtoileVideoPlayer::pause()
{
	_pPlayer->pause();
	ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void EtoileVideoPlayer::stop()
{
	_pPlayer->stop();
	ui.play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	ui.play->setEnabled(false);
	_pPlayer->setVisible(false);
}

void EtoileVideoPlayer::closeEvent(QCloseEvent *event)
{
	_pPlayer->stop();
	_pPlayer->close();
}


void EtoileVideoPlayer::tick(qint64 t)
{
	 QTime displayTime(0, (_pPlayer->currentTime () / 60000) % 60, (_pPlayer->currentTime()/ 1000) % 60);
     ui.playtime->display(displayTime.toString("mm:ss"));
}

void EtoileVideoPlayer::dropEvent(QDropEvent * event)
{
	const QMimeData* mimeData = event->mimeData();

	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		ui.playlist->addSources(urlList);
		ui.play->setEnabled(true);
		play();
	}
}

void EtoileVideoPlayer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void EtoileVideoPlayer::mouseDoubleClickEvent(QMouseEvent * event)
{
	_pPlayer->setVisible(true);
	_pPlayer->raise();
}