#include "SocketConnectionItem.h"
#include <QPen>
#include <QBrush>
#include <QPainter> 

SocketConnectionItem::SocketConnectionItem(QGraphicsItem * parent)
	: QGraphicsLineItem(parent), _pconn(NULL)
{
	//this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->setZValue(0.5);
	loadGUI();
}

SocketConnectionItem::~SocketConnectionItem()
{

}

void SocketConnectionItem::setSocketConnection(Etoile::SocketConnection *conn)
{
	_pconn = conn;
	loadGUI();
}

void SocketConnectionItem::loadGUI()
{
	QPen pen;  // creates a default pen
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(3);
	pen.setBrush(QColor(110,80,80,255));
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);

	this->setPen(pen);
}

void SocketConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(_pconn == NULL)
	{ 
		QPen pen(Qt::darkYellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		painter->setPen(pen);

		float xS = _posS.x();
		float yS = _posS.y();
		float xE = _posE.x();
		float yE = _posE.y();
		float dx = (xE - xS) * 0.5;
		float dy = (yE - yS) * 0.5;

		QPainterPath path(QPointF(xS, yS));
		path.cubicTo(xS + dx, yS, xE - dx, yE,xE,yE);
		painter->drawPath(path);
		//std::cout<<"SocketConnectionItem: "<<std::endl;
		return;
	}
	if(this->isSelected())
	{
		QPen pen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		painter->setPen(pen);
		Etoile::OutputSocket *output =_pconn->getFromSocket();
		Etoile::InputSocket *input = _pconn->getToSocket();
		float xS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).x();
		float yS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).y();
		float xE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).x();
		float yE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).y();
		float dx = (xE - xS) * 0.5;
		float dy = (yE - yS) * 0.5;

		QPainterPath path(QPointF(xS, yS));
		path.cubicTo(xS + dx, yS, xE - dx, yE,xE,yE);
		painter->drawPath(path);
	}
	else
	{
		painter->setPen(this->pen());
		Etoile::OutputSocket *output =_pconn->getFromSocket();
		Etoile::InputSocket *input = _pconn->getToSocket();

		float xS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).x();
		float yS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).y();
		float xE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).x();
		float yE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).y();

		/*float xS = output->getType()._xPos;
		float yS = output->getType()._yPos;
		float xE = input->getType()._xPos;
		float yE = input->getType()._yPos;*/
		float dx = (xE - xS) * 0.5;
		float dy = (yE - yS) * 0.5;

		QPainterPath path(QPointF(xS, yS));
		path.cubicTo(xS + dx, yS, xE - dx, yE,xE,yE);
		painter->drawPath(path);
		//std::cout<<"SocketConnectionItem: "<<xS<<" "<< yS<< " " << xE<< " " << yE<< " " << dx<< " " << dy <<std::endl;
	}

}

QPainterPath SocketConnectionItem::shape() const
{
	if(_pconn == NULL) 
	{
		QPainterPath path;
		QPainterPathStroker stroker;
		stroker.setWidth(3);
		return stroker.createStroke(path);
	}

	Etoile::OutputSocket *output =_pconn->getFromSocket();
	Etoile::InputSocket *input = _pconn->getToSocket();
	if(output == NULL || input== NULL)
	{
		float xS = _posS.x();
		float yS = _posS.y();
		float xE = _posE.x();
		float yE = _posE.y();
		float dx = (xE - xS) * 0.5;
		float dy = (yE - yS) * 0.5;
		QPainterPath path(QPointF(xS, yS));
		path.cubicTo(xS + dx, yS, xE - dx, yE,xE,yE);
		QPainterPathStroker stroker;
		stroker.setWidth(3);
		return stroker.createStroke(path);
	}
	float xS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).x();
	float yS = this->mapFromScene(output->getType()._xPos,output->getType()._yPos).y();
	float xE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).x();
	float yE = this->mapFromScene(input->getType()._xPos,input->getType()._yPos).y();
	float dx = (xE - xS) * 0.5;
	float dy = (yE - yS) * 0.5;
	QPainterPath path(QPointF(xS, yS));
	path.cubicTo(xS + dx, yS, xE - dx, yE,xE,yE);
	QPainterPathStroker stroker;
	stroker.setWidth(3);
	return stroker.createStroke(path);
}

QRectF SocketConnectionItem::boundingRect() const
{
	return shape().boundingRect();
}