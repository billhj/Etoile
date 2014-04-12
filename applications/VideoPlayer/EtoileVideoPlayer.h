#ifndef ETOILEVIDEOPLAYER_H
#define ETOILEVIDEOPLAYER_H

#include <QWidget>
#include "ui_EtoileVideoPlayer.h"
#include "PhononPlayerWidget.h"
#include "PlayListWidget.h"

class EtoileVideoPlayer : public QWidget
{
	Q_OBJECT

public:
	EtoileVideoPlayer(QWidget *parent = 0);
	~EtoileVideoPlayer();
	void openFile(const std::string& name);
	public slots:
		void openFile();
		void play();
		void pause();
		void stop();
		void tick(qint64);
protected:
	void closeEvent(QCloseEvent *event);
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void dropEvent(QDropEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
private:
	Ui::EtoileVideoPlayer ui;
	PhononPlayerWidget * _pPlayer;
	PlayListWidget * _pPlaylistWidget;
	QString _currentMedia;
};

#endif // ETOILEVIDEOPLAYER_H
