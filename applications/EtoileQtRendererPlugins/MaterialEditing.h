#ifndef MaterialEditing_H
#define MaterialEditing_H

#include <QDockWidget>
#include "ui_MaterialEditing.h"
#include "geometry/Material.h"

class MaterialEditing : public QDockWidget
{
	Q_OBJECT
public:
	MaterialEditing(QWidget *parent = 0);
	~MaterialEditing();
	void init(Etoile::Material* material);

public slots:
	void updateParameters();
	void setDiffuseColor();
	void setAmbientColor();
	void setSpecularColor();
private:
	Ui::MaterialEditing ui;
	Etoile::Material* _pMaterial;
};

#endif // MaterialEditing_H
