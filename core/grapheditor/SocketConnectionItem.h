#ifndef SOCKETCONNECTIONITEM_H
#define SOCKETCONNECTIONITEM_H

#include <QGraphicsLineItem>
#include "module/flowgraph/SocketConnection.h"
#include "math/Vec2.h"

class SocketConnectionItem : public QGraphicsLineItem
{
	//Q_OBJECT
public:
	SocketConnectionItem(QGraphicsItem * parent = 0);
	~SocketConnectionItem();
	void setSocketConnection(Etoile::SocketConnection*);
	Etoile::SocketConnection* getSocketConnection(){return _pconn;}
	Etoile::Vec2f& getPosStart(){return _posS;}
	Etoile::Vec2f& getPosEnd(){return _posE;}
protected:
	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	///The rectangle that containg the item, used for rough calculations like
	///collision detection
	QRectF boundingRect() const;

	///More precise shape compared to boundingRect
	///In this example, it is redefined to ease selecting those thin lines
	QPainterPath shape() const;
private:
	void loadGUI();
	Etoile::SocketConnection *_pconn;
	Etoile::Vec2f _posS, _posE;
};

#endif // SOCKETCONNECTIONITEM_H
