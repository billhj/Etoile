#include "PlayListWidget.h"
#include <QUrl>
#include <QMimeData>
#include <QDropEvent>
#include "PhononPlayerWidget.h"

PlayListWidget::PlayListWidget(QWidget *parent)
	: QTableWidget(parent)
{
	QStringList headers;
	headers << tr("Title") << tr("Artist") << tr("Album") << tr("Year") << tr("Path");
	setColumnCount(5);
	setHorizontalHeaderLabels(headers);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	this->resizeColumnsToContents();
	connect(this, SIGNAL(cellPressed(int,int)),
		this, SLOT(tableClicked(int,int)));
	//connect(&mediaobj, SIGNAL(metaDataChanged()), this, SLOT(updateTable()));
}

PlayListWidget::~PlayListWidget()
{

}

void PlayListWidget::setPlayer(PhononPlayerWidget* player)
{
	_pPlayer = player;
	if(player != NULL)
	{
		player->bindPlayList(this);
	}
}

void PlayListWidget::addSources(QList<QUrl> urlList)
{
	// extract the local paths of the files
	for (int i = 0; i < urlList.size() && i < 32; ++i)
	{
		QString mediasource = urlList.at(i).toLocalFile();
		addSource(mediasource);
	}
}

void PlayListWidget::addSource(QString mediasource)
{
	if(_pPlayer == NULL) return;
	if(!mediasource.isEmpty())
	{
		Phonon::MediaSource mediaSource(mediasource);
		if(mediaSource.type() != Phonon::MediaSource::Invalid || mediaSource.type() != Phonon::MediaSource::Empty)
		{
			_sources.append(mediaSource);
			_pPlayer->mediaObject()->setQueue(_sources);		
			updateTable();
			_pPlayer->mediaObject()->setCurrentSource(mediasource);
	
		}
	}
}

void PlayListWidget::updateTable()
{
	if(_pPlayer == NULL) return;
	QMultiMap<QString, QString> data = _pPlayer->mediaObject()->metaData();
	qDebug() << data.keys();  
	QTableWidgetItem *titleItem = new QTableWidgetItem(_pPlayer->mediaObject()->metaData().value("TITLE"));
	//titleItem->setFlags(titleItem->flags() );
	QTableWidgetItem *artistItem = new QTableWidgetItem(data.value("ARTIST"));
	//artistItem->setFlags(artistItem->flags() );
	QTableWidgetItem *albumItem = new QTableWidgetItem(data.value("ALBUM"));
	//albumItem->setFlags(albumItem->flags() );
	QTableWidgetItem *yearItem = new QTableWidgetItem(data.value("DATE"));
	//yearItem->setFlags(yearItem->flags() );
	QTableWidgetItem *pathItem = new QTableWidgetItem(data.value("PATH"));
	//pathItem->setFlags(pathItem->flags() );

	int currentRow = _sources.size();
	this->setRowCount(currentRow);
	this->setItem(currentRow, 0, titleItem);
	this->setItem(currentRow, 1, artistItem);
	this->setItem(currentRow, 2, albumItem);
	this->setItem(currentRow, 3, yearItem);
	this->setItem(currentRow, 4, pathItem);
}

void PlayListWidget::tableClicked(int row, int /* column */)
{
	if (row >= _sources.size())
		return;
	_pPlayer->mediaObject()->setCurrentSource(_sources[row]);
	emit chooseplay();
}

void PlayListWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void PlayListWidget::dropEvent(QDropEvent * event)
{
	const QMimeData* mimeData = event->mimeData();

	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		addSources(urlList);
		emit chooseplay();
	}
}