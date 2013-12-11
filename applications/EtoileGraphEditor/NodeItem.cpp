#include "NodeItem.h"
#include <QPen>
#include <QBrush>
#include <QPainter> 
NodeItem::NodeItem(QGraphicsItem * parent): QGraphicsRectItem(parent), _pnode(NULL)
{
	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
}

NodeItem::~NodeItem()
{

}

void NodeItem::setNode(Etoile::BasicNode *node)
{
	_pnode = node;
	loadGUI();

}

void NodeItem::loadGUI()
{
	if(_pnode == NULL){return;}
	QPen pen;  // creates a default pen
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setBrush(Qt::darkGray);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);

	this->setPen(pen);

	Etoile::NodeType& type = _pnode->getType();
	int r = type._color._r , g = type._color._g, b = type._color._b, a = type._color._a;
	//QBrush brush(QColor(r,g,b,a), Qt::SolidPattern);
	QBrush brush(QColor(r,g,b,a), Qt::Dense2Pattern);
	this->setBrush(brush);
	this->setRadiusX(10);
	this->setRadiusY(10);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(_pnode == NULL){return;}
	if(this->isSelected())
	{
		QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		painter->setBrush(this->brush());
		painter->setPen(pen);
		painter->drawRoundedRect(this->rect(), _radius_x, _radius_y);
		painter->setFont(QFont("Helvetica", 10));
		painter->drawText(this->rect(), Qt::AlignHCenter, QString(_pnode->getType()._name.c_str()));
	}
	else
	{
		painter->setBrush(this->brush());
		painter->setPen(this->pen());
		painter->drawRoundedRect(this->rect(), _radius_x, _radius_y);
		QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		painter->setPen(pen);
		painter->setFont(QFont("Helvetica", 10));
		painter->drawText(this->rect(), Qt::AlignHCenter, QString(_pnode->getType()._name.c_str()));
	}
	Etoile::NodeType& type = _pnode->getType();
	type._xPos = this->mapRectToScene(this->rect()).x();
	type._yPos = this->mapRectToScene(this->rect()).y();
	//this->update();
}