#ifndef SCENENODEEDITING_H
#define SCENENODEEDITING_H

#include <QDockWidget>
#include "ui_SceneNodeEditing.h"
#include "geometry/SceneNode.h"

class SceneNodeEditing : public QDockWidget
{
	Q_OBJECT

public:
	SceneNodeEditing(QWidget *parent = 0);
	~SceneNodeEditing();
	void init(Etoile::SceneNode* scenenode);
public slots:
	void setTransform(double);
	void reflash();
private:
	Ui::SceneNodeEditing ui;
	Etoile::SceneNode* _pSceneNode;
};

#endif // SCENENODEEDITING_H
