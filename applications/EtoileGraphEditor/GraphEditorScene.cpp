#include "GraphEditorScene.h"
#include "NodeItem.h"
#include "SocketConnectionItem.h"
#include "object/SocketNode.h"
#include <QGraphicsSceneMouseEvent>

GraphEditorScene::GraphEditorScene(QObject *parent)
	: QGraphicsScene(parent), _pgraphscene(NULL), _pselectedSocketitem(NULL), _pselectedSocketConnitem(NULL)
{
	connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelections())); 
}

GraphEditorScene::~GraphEditorScene()
{

}

void GraphEditorScene::setGraphScene(Etoile::GraphScene *graphscene)
{
	_pgraphscene = graphscene;
	loadGUI();

}

void GraphEditorScene::loadGUI()
{
	if(_pgraphscene == NULL) return;
	Etoile::Nodes& nodes = _pgraphscene->getNodes();
	Etoile::SocketConnections& conns = _pgraphscene->getSocketConnections();
	for(int i = 0; i < nodes.size(); ++i)
	{
		Etoile::BasicNode *node = nodes[i];
		addNode(node);
	}

	for(int i = 0; i < conns.size(); ++i)
	{
		Etoile::SocketConnection *conn = conns[i];
		SocketConnectionItem* sitem = new SocketConnectionItem();
		sitem->setSocketConnection(conn);
		this->addItem(sitem);
	}
	_connecting = new SocketConnectionItem();
	this->addItem(_connecting);
}

void GraphEditorScene::addNode(Etoile::BasicNode *node)
{
	int idx = _pgraphscene->getNodeIndex(node);
	if(idx < 0) _pgraphscene->addNode(node);
	NodeItem *nitem = new NodeItem();
	Etoile::NodeType& typeNode = node->getType();
	nitem->setRect(typeNode._xPos, typeNode._yPos, typeNode._w, typeNode._h);
	nitem->setNode(node);
	this->addItem(nitem);

	Etoile::SocketNode * snode = dynamic_cast<Etoile::SocketNode*>(node);
	if(snode != NULL)
	{
		Etoile::InputSockets inputs = snode->getInputSockets();
		for(int i = 0; i < inputs.size(); ++i)
		{	
			Etoile::InputSocket *input = inputs[i];
			input->getType()._xPos = -typeNode._w * 0.1 + typeNode._xPos;
			input->getType()._yPos = typeNode._h * (i + 1) * 0.1 + typeNode._yPos;
			SocketItem* item = new SocketItem(nitem);
			Etoile::SocketType& type = input->getType();
			item->setRect(type._xPos, type._yPos, type._w, type._h);
			//item->setParentItem(nitem);
			item->setSocket(input);

		}

		Etoile::OutputSockets outputs = snode->getOutputSockets();
		for(int i = 0; i < outputs.size(); ++i)
		{	
			Etoile::OutputSocket *output = outputs[i];
			output->getType()._xPos = typeNode._w * 0.8 + typeNode._xPos;
			output->getType()._yPos = typeNode._h - typeNode._h * (i + 1) * 0.1 + typeNode._yPos;
			SocketItem* item = new SocketItem(nitem);
			Etoile::SocketType& type = output->getType();
			item->setRect(type._xPos, type._yPos, type._w, type._h);
			//item->setParentItem(nitem);
			item->setSocket(output);
		}
	}
}

void GraphEditorScene::removeSocketConnection(Etoile::SocketConnection *conn)
{
	_pgraphscene->disconnect(conn);
}

void GraphEditorScene::checkSelections()
{
	QList<QGraphicsItem *> items =  this->selectedItems();
	SocketItem * second = NULL;
	foreach(QGraphicsItem* item, items)
	{
		SocketItem* a = dynamic_cast<SocketItem*>(item);
		if(a != NULL)
			second = a;
		_pselectedSocketConnitem = dynamic_cast<SocketConnectionItem*>(item);
	}

	if(_pselectedSocketitem == NULL && second != NULL)
	{
		_pselectedSocketitem = second;
	}
	else if(_pselectedSocketitem != second && second != NULL)
	{
		_pselectedSocketitem = second;
	}
	else
	{
		_pselectedSocketitem = NULL;
	}
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<<"Qt scene item size: "<<this->items().size()<<std::endl;
#endif
}

void GraphEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//if(event-)
	if(_pselectedSocketitem != NULL && _pselectedSocketitem->isSelected())
	{
		Etoile::Vec2f& posS =_connecting->getPosStart();
		Etoile::Vec2f& posE =_connecting->getPosEnd();
		//posS.set(_pselectedSocketitem->mapRectToScene(_pselectedSocketitem->rect()).x(), _pselectedSocketitem->mapRectToScene(_pselectedSocketitem->rect()).y());
		posS.set(_pselectedSocketitem->getSocket()->getType()._xPos, _pselectedSocketitem->getSocket()->getType()._yPos);
		posE.set(event->scenePos().x(), event->scenePos().y());
		//std::cout<<"move"<<posS <<posE<<std::endl;
		_connecting->update();
	}
	else
	{
		Etoile::Vec2f& posS =_connecting->getPosStart();
		Etoile::Vec2f& posE =_connecting->getPosEnd();
		posS.set(0,0);
		posE.set(0,0);
	}
	QGraphicsScene::mouseMoveEvent(event);
}

void GraphEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if(_pselectedSocketitem != NULL && _pselectedSocketitem->isSelected())
	{
		Etoile::Vec2f& posS =_connecting->getPosStart();
		Etoile::Vec2f& posE =_connecting->getPosEnd();
		posS.set(0,0);
		posE.set(0,0);

		SocketItem * second = NULL;
		foreach(QGraphicsItem* item, this->items())
		{
			second = dynamic_cast<SocketItem*>(item);
			if(second != NULL && _pselectedSocketitem != second)
			{
				if(second->contains(second->mapFromScene(event->scenePos())))
				{
					Etoile::Socket* s0 = _pselectedSocketitem->getSocket();
					Etoile::Socket* s1 = second->getSocket();
					if(s0->verify(s1) && s0->getUsageType()!=s1->getUsageType())
					{
						Etoile::InputSocket * input;
						Etoile::OutputSocket * output;
						if(s0->getUsageType()==Etoile::INPUT)
						{
							input = (Etoile::InputSocket*)s0;
							output = (Etoile::OutputSocket*)s1;
						}
						else
						{
							input = (Etoile::InputSocket*)s1;
							output = (Etoile::OutputSocket*)s0;
						}
						Etoile::SocketConnection *conn = _pgraphscene->getSocketConnection(input);
						if(conn == NULL || conn->getFromSocket() != output)
						{
							//first delete the old conn
							if(conn != NULL && conn->getFromSocket() != output)
							{
								foreach(QGraphicsItem* item, this->items())
								{
									SocketConnectionItem * it = dynamic_cast<SocketConnectionItem*>(item);
									if(it != NULL && it->getSocketConnection() == conn)
									{
										it->hide();
										this->removeItem(it);
										removeSocketConnection(conn);
									}
								}

							}
							conn = new Etoile::SocketConnection();
							conn->setToSocket(input);
							conn->setFromSocket(output);
							_pgraphscene->connect(conn);
							SocketConnectionItem* sitem = new SocketConnectionItem();
							sitem->setSocketConnection(conn);
							this->addItem(sitem);
							//sitem->show();
						}

					}
				}
			}
		}
	}

	QGraphicsScene::mouseReleaseEvent(event);
}

void GraphEditorScene::keyPressEvent(QKeyEvent * event)
{
	if(event->key() == Qt::Key_Delete)
	{
		if(_pselectedSocketConnitem != NULL)
		{
			Etoile::SocketConnection* conn = _pselectedSocketConnitem->getSocketConnection();
			_pselectedSocketConnitem->hide();
			this->removeItem(_pselectedSocketConnitem);
			removeSocketConnection(conn);
		}
	}
	QGraphicsScene::keyPressEvent(event);
}