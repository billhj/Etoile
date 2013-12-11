#ifndef GRAPHEDITORSCENE_H
#define GRAPHEDITORSCENE_H

#include <QGraphicsScene>
#include "module/flowgraph/GraphScene.h"
#include "SocketConnectionItem.h"
#include "SocketItem.h"
#include "NodeItem.h"
#include <QKeyEvent>

class GraphEditorScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GraphEditorScene(QObject *parent);
	~GraphEditorScene();
	void setGraph(Etoile::GraphScene *graphscene);
	Etoile::GraphScene* getGraph(){return _pgraphscene;}
	void addNode(Etoile::BasicNode *node, NodeItem *nitems);
	void addConnection(Etoile::OutputSocket*, Etoile::InputSocket*);
	void removeSocketConnection(Etoile::SocketConnection *conn);
	void updateGUI();
	public slots:
		void checkSelections();
		
protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);
private:
	void loadGUI();
	Etoile::GraphScene *_pgraphscene;
	SocketItem *_pselectedSocketitem;
	QGraphicsItem *_pselecteditem;
	SocketConnectionItem* _connecting;
};

#endif // GRAPHEDITORSCENE_H
