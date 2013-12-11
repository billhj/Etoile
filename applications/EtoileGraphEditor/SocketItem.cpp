#include "SocketItem.h"
#include <QPen>
#include <QBrush>
#include <QPainter> 
#include <iostream>

SocketItem::SocketItem(QGraphicsItem * parent): QGraphicsRectItem(parent), _psocket(NULL)
{
		//this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->setZValue(1);
}

SocketItem::~SocketItem()
{

}

void SocketItem::setSocket(Etoile::Socket* socket)
{
	_psocket = socket;
	loadGUI();
}

void SocketItem::loadGUI()
{
	if(_psocket == NULL) return;
	Etoile::SocketType& type = _psocket->getType();
	int r = type._color._r , g = type._color._g, b = type._color._b, a = type._color._a;
	QBrush brush(QColor(r,g,b,a), Qt::SolidPattern);
	//QBrush brush(Qt::red, Qt::SolidPattern);
	this->setBrush(brush);
	this->setRadiusX(30);
	this->setRadiusY(30);
}

void SocketItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(_psocket == NULL) return;
	if(this->isSelected())
	{
		Etoile::SocketType& type = _psocket->getType();
		int r = 180, g = 30, b = 0, a = 255;
		QBrush brush(QColor(r,g,b,a), Qt::SolidPattern);
		painter->setBrush(brush);
		painter->setPen(this->pen());
		painter->drawRoundedRect(this->rect(), _radius_x, _radius_y);
		painter->drawText(this->rect().x(), this->rect().y(), 100, 80, Qt::AlignHCenter, QString(_psocket->getType()._name.c_str()));
	}
	else
	{
		painter->setBrush(this->brush());
		painter->setPen(this->pen());
		painter->drawRoundedRect(this->rect(), _radius_x, _radius_y);
		//painter->setFont(QFont("Verdana", 7)); 
		painter->drawText(this->rect().x(), this->rect().y(), 100, 80, Qt::AlignHCenter, QString(_psocket->getType()._name.c_str()));
	}
	Etoile::SocketType& type = _psocket->getType();
	type._xPos = this->mapRectToScene(this->rect()).center().x();
	type._yPos = this->mapRectToScene(this->rect()).center().y();

	/*if(this->isSelected())
	 std::cout<<"SocketItem: "<<type._xPos<<" "<< type._yPos<<std::endl;*/
	//this->update();
}
