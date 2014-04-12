#include "GraphEditorScene.h"
#include "SocketConnectionItem.h"
#include "module/flowgraph/SocketNode.h"
#include <QGraphicsSceneMouseEvent>

GraphEditorScene::GraphEditorScene(QObject *parent)
	: QGraphicsScene(parent), _pgraphscene(NULL), _pselectedSocketitem(NULL), _pselecteditem(NULL), _connecting(NULL)
{
	connect(this, SIGNAL(selectionChanged()), this, SLOT(checkSelections())); 
	_pgraphscene = new Etoile::GraphScene();
	_connecting = new SocketConnectionItem();
	this->addItem(_connecting);
}

GraphEditorScene::~GraphEditorScene()
{
	delete _pgraphscene;
}

void GraphEditorScene::setGraph(Etoile::GraphScene *graphscene)
{
	_pgraphscene = graphscene;
	loadGUI();
}


void GraphEditorScene::clearScene()
{
	_pgraphscene->clear();
	loadGUI();
}


void GraphEditorScene::updateGUI()
{
	loadGUI();
}

void GraphEditorScene::loadGUI()
{
	if(_pgraphscene == NULL) return;
	this->removeItem(_connecting);
	this->clear();
	//_connecting = new SocketConnectionItem();
	this->addItem(_connecting);

	Etoile::Nodes& nodes = _pgraphscene->getNodes();
	Etoile::SocketConnections& conns = _pgraphscene->getSocketConnections();
	for(int i = 0; i < nodes.size(); ++i)
	{
		Etoile::BasicNode *node = nodes[i];
		NodeItem *nitem = new NodeItem();
		addNode(node, nitem);
	}

	for(int i = 0; i < conns.size(); ++i)
	{
		Etoile::SocketConnection *conn = conns[i];
		SocketConnectionItem* sitem = new SocketConnectionItem();
		sitem->setSocketConnection(conn);
		this->addItem(sitem);
	}

}

void GraphEditorScene::addNode(Etoile::BasicNode *node, NodeItem *nitem)
{
	int idx = _pgraphscene->getNodeIndex(node);
	if(idx < 0)
	{
		_pgraphscene->addNode(node);
	}
	Etoile::NodeType& typeNode = node->getType();
	nitem->setRect(typeNode._xPos, typeNode._yPos, typeNode._w, typeNode._h);
	nitem->setNode(node);
	this->addItem(nitem);	
}

void GraphEditorScene::addConnection(Etoile::OutputSocket* output, Etoile::InputSocket* input)
{
	if(output == NULL || input == NULL) return;
	if(output->verify(input) && output->getUsageType()!=input->getUsageType())
	{
		Etoile::SocketConnection *conn = _pgraphscene->getSocketConnection(input);
		if(conn == NULL || conn->getFromSocket() != output)
		{
			//first delete the old conn
			if(conn != NULL && conn->getFromSocket() != output && input->getUsageType() == Etoile::INPUT_UNI)
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
		_pselecteditem = dynamic_cast<QGraphicsItem*>(item);
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
	if(_pselectedSocketitem != NULL && _pselectedSocketitem->isSelected() && _connecting != NULL)
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
						if(s0->getUsageType()==Etoile::INPUT_UNI || s0->getUsageType()==Etoile::INPUT_MUL)
						{
							input = (Etoile::InputSocket*)s0;
							output = (Etoile::OutputSocket*)s1;
						}
						else
						{
							input = (Etoile::InputSocket*)s1;
							output = (Etoile::OutputSocket*)s0;
						}
						addConnection(output, input);
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
		if(_pselecteditem != NULL)
		{
			SocketConnectionItem * it = dynamic_cast<SocketConnectionItem*>(_pselecteditem);
			if(it != NULL)
			{
				Etoile::SocketConnection* conn = it->getSocketConnection();
				it->hide();
				this->removeItem(it);
				removeSocketConnection(conn);
			}

			NodeItem * itnode = dynamic_cast<NodeItem*>(_pselecteditem);
			if(itnode != NULL)
			{
				Etoile::BasicNode *node = itnode->getNode();
				foreach(QGraphicsItem* item, this->items())
				{
					SocketConnectionItem * it = dynamic_cast<SocketConnectionItem*>(item);
					if(it != NULL)
					{
						Etoile::SocketConnection* conn = it->getSocketConnection();
						if(conn != NULL)
						{
							if(conn->getFromNode() == node || conn->getToNode() == node)
							{
								item->hide();
								this->removeItem(item);
							}
						}
					}
				}
				itnode->hide();
				this->removeItem(itnode);
				_pgraphscene->removeNode(node);

			}
		}
	}
	QGraphicsScene::keyPressEvent(event);
}