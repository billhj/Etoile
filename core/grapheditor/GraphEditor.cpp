#include "GraphEditor.h"
#include <QMenu>
#include <QString>

GraphEditor::GraphEditor(QWidget *parent)
	: QGraphicsView(parent)
{
	QBrush brush(Qt::darkGray, Qt::CrossPattern);
	setBackgroundBrush(brush);
	_pScene = new GraphEditorScene(this);
	this->setScene(_pScene);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing, true);

	//QAction* addNodeAction = new QAction(QString("add Node"), this);
	//connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
	//_actions.append(addNodeAction);

	//test
	/*{
		Etoile::GraphScene* scene = new Etoile::GraphScene();
		Etoile::SocketNode* node = new Etoile::SocketNode();
		node->getType()._xPos = 50;
		node->getType()._yPos = 50;
		node->getType()._name = "first";

		Etoile::InputSocket* in0 = new Etoile::InputSocket();
		in0->initType(Etoile::SOCK_RGBA);
		Etoile::OutputSocket* out0 = new Etoile::OutputSocket();
		out0->initType(Etoile::SOCK_STRING);
		node->addInputSocket(in0);
		node->addOutputSocket(out0);

		scene->addNode(node);

		node = new Etoile::SocketNode();
		node->getType()._xPos = 250;
		node->getType()._yPos = 250;
		node->getType()._name = "second";

		Etoile::InputSocket* in1 = new Etoile::InputSocket();
		in1->initType(Etoile::SOCK_STRING);
		Etoile::OutputSocket* out1 = new Etoile::OutputSocket();
		out1->initType(Etoile::SOCK_RGBA);
		node->addInputSocket(in1);
		node->addOutputSocket(out1);

		scene->addNode(node);
		scene->connect(out0, in1);

		_pScene->setGraphScene(scene);
	}*/
}

GraphEditor::~GraphEditor()
{

}

void GraphEditor::setGraph(Etoile::GraphScene* scene)
{
	if(scene != NULL){
		_pScene->setGraph(scene);
	}
}

void GraphEditor::clearGraphScene()
{
	_pScene->clearScene();
}

Etoile::GraphScene * GraphEditor::getGraph()
{
	if(_pScene != NULL){
		return _pScene->getGraph();
	}
	return NULL;
}

void GraphEditor::wheelEvent(QWheelEvent *event)
{
	if(event->delta()>0)
	{
		this->scale(1.01,1.01);
	}
	else
	{
		this->scale(0.99,0.99);
	}
}

void GraphEditor::mouseDoubleClickEvent(QMouseEvent * event)
{
	//x = this->mapToScene(event->pos().x(), event->pos().y()).x();
	//y = this->mapToScene(event->pos().x(), event->pos().y()).y();
	/*if(event->button() == Qt::RightButton)
	{
		QMenu MyMenu(this);
		MyMenu.addAction("update");
		
		QAction* a = MyMenu.exec(event->globalPos());
		//std::cout<<"GraphEditor: "<<x<<" "<<y<<std::endl;
		//event->accept();
		if(a != NULL)
		{
			_pScene->updateGUI();
		}
	}
	else
	{
	}*/
	_pScene->updateGUI();
	QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphEditor::addNode(Etoile::SocketNode *node)
{
	NodeItem *nitem = new NodeItem();
	_pScene->addNode(node, nitem);
}

void GraphEditor::addNode(Etoile::SocketNode* node, NodeItem * nitem)
{
	_pScene->addNode(node, nitem);
}

void GraphEditor::addConnection(Etoile::OutputSocket* output, Etoile::InputSocket* input)
{
	_pScene->addConnection(output, input);
}