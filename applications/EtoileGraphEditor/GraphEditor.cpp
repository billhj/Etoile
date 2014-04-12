#include "GraphEditor.h"
#include "object/SocketNode.h"
#include <QMenu>
#include <QString>

GraphEditor::GraphEditor(QWidget *parent)
	: QGraphicsView(parent)
{
	ui.setupUi(this);
	QBrush brush(Qt::darkGray, Qt::CrossPattern);
	setBackgroundBrush(brush);
	_pScene = new GraphEditorScene(this);
	this->setScene(_pScene);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing, true);

	QAction* addNodeAction = new QAction(QString("add Node"), this);
	connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
	_actions.append(addNodeAction);

	//test
	{
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
	}
}

GraphEditor::~GraphEditor()
{

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

void GraphEditor::mouseReleaseEvent(QMouseEvent * event)
{
	x = this->mapToScene(event->pos().x(), event->pos().y()).x();
	y = this->mapToScene(event->pos().x(), event->pos().y()).y();
	if(event->button() == Qt::RightButton)
	{
		QMenu MyMenu(this);
		MyMenu.addAction("aaa");
		MyMenu.addAction("bbb");
		MyMenu.addActions(_actions);
		MyMenu.exec(event->globalPos());
		//std::cout<<"GraphEditor: "<<x<<" "<<y<<std::endl;
		event->accept();
	}
	else
	{
	}
	
	QGraphicsView::mouseReleaseEvent(event);
}

void GraphEditor::addNode()
{
	Etoile::SocketNode* node = new Etoile::SocketNode();
	node->getType()._xPos = x;
	node->getType()._yPos = y;
	node->getType()._name = "third";

	Etoile::InputSocket* in0 = new Etoile::InputSocket();
	in0->initType(Etoile::SOCK_RGBA);
	Etoile::OutputSocket* out0 = new Etoile::OutputSocket();
	out0->initType(Etoile::SOCK_STRING);
	node->addInputSocket(in0);
	node->addOutputSocket(out0);
	_pScene->addNode(node);

}