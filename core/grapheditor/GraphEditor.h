#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QGraphicsView>
#include "GraphEditorScene.h"
#include <QWheelEvent>
#include "module/flowgraph/SocketNode.h"

class GraphEditor : public QGraphicsView
{
	Q_OBJECT

public:
	GraphEditor(QWidget *parent = 0);
	~GraphEditor();
	void setGraph(Etoile::GraphScene*);
	Etoile::GraphScene * getGraph();
	public slots:
	void addNode(Etoile::SocketNode*);
	void addNode(Etoile::SocketNode*, NodeItem *);
	void addConnection(Etoile::OutputSocket*, Etoile::InputSocket*);
protected:
	virtual void wheelEvent(QWheelEvent *event); 
	virtual void mouseDoubleClickEvent(QMouseEvent * event);
private:
	GraphEditorScene* _pScene;
	QList<QAction*> _actions;
	float x, y;
};

#endif // GRAPHEDITOR_H
