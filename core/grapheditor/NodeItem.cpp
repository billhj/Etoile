#include "NodeItem.h"
#include <QPen>
#include <QBrush>
#include <QPainter> 
#include "module/EPlugin.h"
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include "module/flowgraph/SocketNode.h"
#include "SocketItem.h"

NodeItem::NodeItem(QGraphicsItem * parent): QGraphicsRectItem(parent), _pnode(NULL)
{
	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	QAction* init = new QAction(QString("init"), this->parentObject());
	QAction* apply = new QAction(QString("apply"), this->parentObject());
	QAction* release = new QAction(QString("release"), this->parentObject());
	connect(init, SIGNAL(triggered()), this, SLOT(init()));
	connect(apply, SIGNAL(triggered()), this, SLOT(apply()));
	connect(release, SIGNAL(triggered()), this, SLOT(release()));
	_actions.append(init);
	_actions.append(apply);
	_actions.append(release);
}

NodeItem::~NodeItem()
{

}

void NodeItem::setNode(Etoile::BasicNode *node)
{
	_pnode = node;
	loadGUI();

}

void NodeItem::updateGUI()
{
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

	Etoile::SocketNode * snode = dynamic_cast<Etoile::SocketNode*>(_pnode);
	if(snode != NULL)
	{
		Etoile::InputSockets inputs = snode->getInputSockets();
		for(int i = 0; i < inputs.size(); ++i)
		{	
			Etoile::InputSocket *input = inputs[i];
			input->getType()._xPos = -type._w * 0.1 + type._xPos;
			input->getType()._yPos = 120 * (i + 1) * 0.1 + type._yPos;
			SocketItem* item = new SocketItem(this);
			Etoile::SocketType& type = input->getType();
			item->setRect(type._xPos, type._yPos, type._w, type._h);
			//item->setParentItem(nitem);
			item->setSocket(input);

		}

		Etoile::OutputSockets outputs = snode->getOutputSockets();
		for(int i = 0; i < outputs.size(); ++i)
		{	
			Etoile::OutputSocket *output = outputs[i];
			output->getType()._xPos = type._w * 0.8 + type._xPos;
			output->getType()._yPos = type._h - 120 * (i + 1) * 0.1 + type._yPos;
			SocketItem* item = new SocketItem(this);
			Etoile::SocketType& type = output->getType();
			item->setRect(type._xPos, type._yPos, type._w, type._h);
			//item->setParentItem(nitem);
			item->setSocket(output);
		}
	}
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

void NodeItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
	Etoile::EPlugin * plug = dynamic_cast<Etoile::EPlugin*>(this->getNode());
	if(plug != NULL)
	{
		plug->apply();
	}
	QGraphicsRectItem::mouseDoubleClickEvent(event);
}

void NodeItem::mousePressEvent  ( QGraphicsSceneMouseEvent * event )
{
	if(event->button() == Qt::MouseButton::RightButton)
	{
		if(this->rect().contains(event->pos()))
		{
			QMenu MyMenu;
			MyMenu.addActions(_actions);
			MyMenu.exec(event->screenPos());	
		}
	}
	QGraphicsRectItem::mousePressEvent(event);
}

void NodeItem::init()
{
	Etoile::EPlugin * plug = dynamic_cast<Etoile::EPlugin*>(this->getNode());
	if(plug != NULL)
	{
		plug->init();
	}
}
void NodeItem::apply()
{
	Etoile::EPlugin * plug = dynamic_cast<Etoile::EPlugin*>(this->getNode());
	if(plug != NULL)
	{
		plug->apply();
	}
}

void NodeItem::release()
{
	Etoile::EPlugin * plug = dynamic_cast<Etoile::EPlugin*>(this->getNode());
	if(plug != NULL)
	{
		plug->release();
	}
}