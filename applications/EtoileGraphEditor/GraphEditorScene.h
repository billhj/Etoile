#ifndef GRAPHEDITORSCENE_H
#define GRAPHEDITORSCENE_H

#include <QGraphicsScene>
#include "object/GraphScene.h"
#include "SocketConnectionItem.h"
#include "SocketItem.h"
#include <QKeyEvent>

class GraphEditorScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GraphEditorScene(QObject *parent);
	~GraphEditorScene();
	void setGraphScene(Etoile::GraphScene *graphscene);
	Etoile::GraphScene* getGraphScene(){return _pgraphscene;}
	void addNode(Etoile::BasicNode *node);
	void removeSocketConnection(Etoile::SocketConnection *conn);
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
	SocketConnectionItem *_pselectedSocketConnitem;
	SocketConnectionItem* _connecting;
};

#endif // GRAPHEDITORSCENE_H
