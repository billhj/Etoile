#ifndef PhononPlayerWidget_H
#define PhononPlayerWidget_H

#include <QWidget>
#include <Phonon/VideoPlayer>


class PlayListWidget;
class PhononPlayerWidget : public Phonon::VideoPlayer
{
	Q_OBJECT
public:
	PhononPlayerWidget(Phonon::Category category = Phonon::VideoCategory, QWidget * parent = 0 );
	~PhononPlayerWidget();	
	void bindPlayList(PlayListWidget*);
protected:
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void dropEvent(QDropEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
private:
	Phonon::VideoPlayer * _pPlayer;
	PlayListWidget * _pPlaylist;
};

#endif // PhononPlayerWidget_H
