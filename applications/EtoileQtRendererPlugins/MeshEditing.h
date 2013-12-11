#ifndef MESHEDITING_H
#define MESHEDITING_H

#include <QWidget>
#include "ui_MeshEditing.h"
#include "geometry/Mesh.h"

class MeshEditing : public QWidget
{
	Q_OBJECT

public:
	MeshEditing(QWidget *parent = 0);
	~MeshEditing();
	void init(Etoile::Mesh* mesh);
	
	Etoile::SubMesh* getCurrentSubMesh();
	std::string getMeshRenderUnitType();
public slots:
	void updateParameters(int index);
	void setDiffuseColor();
	void setAmbientColor();
	void setSpecularColor();
	void setTransform(double);
	void setMeshTransform(double);
	void reflash();
private:
	Ui::MeshEditing ui;
	Etoile::Mesh* _pMesh;
};

#endif // MESHEDITING_H
