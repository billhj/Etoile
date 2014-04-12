#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QGraphicsView>
#include "ui_GraphEditor.h"
#include "GraphEditorScene.h"
#include <QWheelEvent>

class GraphEditor : public QGraphicsView
{
	Q_OBJECT

public:
	GraphEditor(QWidget *parent = 0);
	~GraphEditor();
	public slots:
	void addNode();
protected:
	virtual void wheelEvent(QWheelEvent *event); 
	virtual void mouseReleaseEvent(QMouseEvent * event);
private:
	Ui::GraphEditor ui;
	GraphEditorScene* _pScene;
	QList<QAction*> _actions;
	float x, y;
};

#endif // GRAPHEDITOR_H
