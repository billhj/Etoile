#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QTableWidget>
#include <phonon/mediaobject>
#include <QList>

class PhononPlayerWidget;
class PlayListWidget : public QTableWidget
{
	Q_OBJECT

public:
	PlayListWidget(QWidget *parent = 0);
	~PlayListWidget();
	void setPlayer(PhononPlayerWidget* player); 
	void addSources(QList<QUrl> urlList);
	void addSource(QString mediasource);

	public slots:
	void updateTable();
	void tableClicked(int row, int /* column */);
signals:
	void chooseplay();
protected:
	virtual void dropEvent(QDropEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
private:
	PhononPlayerWidget* _pPlayer;
	QList<Phonon::MediaSource> _sources;
};

#endif // PLAYLISTWIDGET_H
