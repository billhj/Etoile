#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsRectItem>
#include "object/BasicGraph.h"
//#include "SocketItem.h"

class NodeItem : public QGraphicsRectItem 
{
	//Q_OBJECT
public:
	NodeItem(QGraphicsItem * parent = 0);
	~NodeItem();
	void setNode(Etoile::BasicNode*);
	Etoile::BasicNode* getNode(){return _pnode;}

	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	///Get the rounded rect corner x radius
	double getRadiusX() const { return _radius_x; }

	///Get the rounded rect corner y radius
	double getRadiusY() const { return _radius_y; }

	///Set the rounded rect corner x radius
	void setRadiusX(const double radius_x) { _radius_x = radius_x; }

	///Set the rounded rect corner y radius
	void setRadiusY(const double radius_y) { _radius_y = radius_y; }
private:
	void loadGUI();
	Etoile::BasicNode *_pnode;
	//std::vector<SocketItem*> _children;

	///The rounded rect corner x radius
	double _radius_x;

	///The rounded rect corner y radius
	double _radius_y;
};

#endif // NODEITEM_H
