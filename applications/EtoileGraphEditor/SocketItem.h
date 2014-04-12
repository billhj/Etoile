#ifndef SOCKETITEM_H
#define SOCKETITEM_H

#include <QGraphicsRectItem>
#include "object/Socket.h"

class SocketItem : public QGraphicsRectItem
{
	//Q_OBJECT
public:
	SocketItem(QGraphicsItem * parent = 0);
	virtual ~SocketItem();
	void setSocket(Etoile::Socket*);
	Etoile::Socket* getSocket(){return _psocket;}

	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	///Get the rounded rect corner x radius
	double getRadiusX() const { return _radius_x; }

	///Get the rounded rect corner y radius
	double getRadiusY() const { return _radius_y; }

	///Set the rounded rect corner x radius
	void setRadiusX(const double radius_x) { _radius_x = radius_x; }

	///Set the rounded rect corner y radius
	void setRadiusY(const double radius_y) { _radius_y = radius_y; }
protected:
	//virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
private:
	void loadGUI();
	Etoile::Socket *_psocket;

	///The rounded rect corner x radius
	double _radius_x;

	///The rounded rect corner y radius
	double _radius_y;
};
#endif // SOCKETITEM_H
