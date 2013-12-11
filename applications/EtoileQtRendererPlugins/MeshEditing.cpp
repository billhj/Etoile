#include "MeshEditing.h"
#include <QPalette>
#include <QColorDialog>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

MeshEditing::MeshEditing(QWidget *parent)
	: QWidget(parent), _pMesh(NULL)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	connect(ui.submesh, SIGNAL(currentIndexChanged(int)), this, SLOT(updateParameters(int)));
	connect(ui.diffuseC, SIGNAL(clicked()), this, SLOT(setDiffuseColor()));
	connect(ui.specularC, SIGNAL(clicked()), this, SLOT(setSpecularColor()));
	connect(ui.ambientC, SIGNAL(clicked()), this, SLOT(setAmbientColor()));

	connect(ui.rotateX,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateY,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateZ,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateAngle,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateX,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateY,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateZ,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleX,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleY,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleZ,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));


	connect(ui.rotateX_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.rotateY_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.rotateZ_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.rotateAngle_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.translateX_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.translateY_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.translateZ_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.scaleX_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.scaleY_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));
	connect(ui.scaleZ_2,SIGNAL(valueChanged(double)), this, SLOT(setMeshTransform(double)));

	
	ui.submeshTab->setTabText(0,"Material");
	ui.submeshTab->setTabText(1,"Transform");

	ui.toolBox->setItemText(0, "Mesh");
	ui.toolBox->setItemText(1, "SubMesh");

	ui.renderunit->addItem("RenderMode_Immediate");
	ui.renderunit->addItem("RenderMode_GPUVBO");
	ui.renderunit->addItem("RenderMode_AvancedGPUVBO");
	ui.renderunit->setCurrentIndex(2);

	connect(ui.update, SIGNAL(clicked()), this, SLOT(reflash()));
}

MeshEditing::~MeshEditing()
{

}

std::string MeshEditing::getMeshRenderUnitType()
{
	return ui.renderunit->currentText().toStdString();
}

void MeshEditing::init(Etoile::Mesh* mesh)
{
	_pMesh = mesh;
	if(_pMesh == NULL) return;
	this->setWindowTitle(QString(("MeshEditing: "+_pMesh->getName()).c_str()));
	ui.submesh->clear();
	const std::vector<Etoile::SubMesh*>& submeshlist = _pMesh->getSubMeshList();

	for(unsigned int i = 0; i < submeshlist.size(); ++i)
	{
		Etoile::SubMesh* submesh = submeshlist[i];
		ui.submesh->addItem(QString(submesh->getName().c_str()));
	}

	Etoile::Vec3f scale = _pMesh->getScale();
	ui.scaleX_2->setValue(scale.x());
	ui.scaleY_2->setValue(scale.y());
	ui.scaleZ_2->setValue(scale.z());

	Etoile::Quaternionf q = _pMesh->getRotation();
	Etoile::Vec3f axis = q.axis();
	ui.rotateX_2->setValue(axis.x());
	ui.rotateY_2->setValue(axis.y());
	ui.rotateZ_2->setValue(axis.z());
	ui.rotateAngle_2->setValue(q.angle());

	Etoile::Vec3f trans = _pMesh->getTranslation();
	ui.translateX_2->setValue(trans.x());
	ui.translateY_2->setValue(trans.y());
	ui.translateZ_2->setValue(trans.z());
}

void MeshEditing::reflash()
{
	if(_pMesh == NULL) return;
	ui.scaleX_2->blockSignals(true);
	ui.scaleY_2->blockSignals(true);
	ui.scaleZ_2->blockSignals(true);
	ui.rotateX_2->blockSignals(true);
	ui.rotateY_2->blockSignals(true);
	ui.rotateZ_2->blockSignals(true);
	ui.rotateAngle_2->blockSignals(true);
	ui.translateX_2->blockSignals(true);
	ui.translateY_2->blockSignals(true);
	ui.translateZ_2->blockSignals(true);

	Etoile::Vec3f scale = _pMesh->getScale();
	ui.scaleX_2->setValue(scale.x());
	ui.scaleY_2->setValue(scale.y());
	ui.scaleZ_2->setValue(scale.z());

	Etoile::Quaternionf q = _pMesh->getRotation();
	Etoile::Vec3f axis = q.axis();
	ui.rotateX_2->setValue(axis.x());
	ui.rotateY_2->setValue(axis.y());
	ui.rotateZ_2->setValue(axis.z());
	ui.rotateAngle_2->setValue(q.angle());

	Etoile::Vec3f trans = _pMesh->getTranslation();
	ui.translateX_2->setValue(trans.x());
	ui.translateY_2->setValue(trans.y());
	ui.translateZ_2->setValue(trans.z());
	//blockSignals(false);
	ui.scaleX_2->blockSignals(false);
	ui.scaleY_2->blockSignals(false);
	ui.scaleZ_2->blockSignals(false);
	ui.rotateX_2->blockSignals(false);
	ui.rotateY_2->blockSignals(false);
	ui.rotateZ_2->blockSignals(false);
	ui.rotateAngle_2->blockSignals(false);
	ui.translateX_2->blockSignals(false);
	ui.translateY_2->blockSignals(false);
	ui.translateZ_2->blockSignals(false);
}

void MeshEditing::updateParameters(int index)
{
	if(_pMesh == NULL) return;
	Etoile::SubMesh* current = NULL;
	const std::vector<Etoile::SubMesh*>& submeshlist = _pMesh->getSubMeshList();
	if(index < submeshlist.size() && index > -1)
	{
		current = submeshlist[index];
		Etoile::Material* m = current->getMaterial();
		if(m != NULL)
		{
			const Etoile::Vec4f& dif = m->getDiffuse();
			const Etoile::Vec4f& spe = m->getSpecular();
			const Etoile::Vec4f& amb = m->getAmbient();
			//Etoile::Vec4f dif(1,0,0,1);
			{
				QPalette palette;
				palette.setColor(QPalette::ButtonText, QColor::fromRgbF(amb.r(),amb.g(),amb.b(),amb.a()));
				ui.ambientC->setPalette(palette);
				ui.ambientC->setAutoFillBackground(true);
				QPalette palette2;
				palette2.setColor(ui.dif->backgroundRole(), QColor::fromRgbF(amb.r(),amb.g(),amb.b(),amb.a()));
				ui.amb->setPalette(palette2);
				ui.amb->setAutoFillBackground(true);
			}
			{
				QPalette palette;
				palette.setColor(QPalette::ButtonText, QColor::fromRgbF(dif.r(),dif.g(),dif.b(),dif.a()));
				ui.diffuseC->setPalette(palette);
				ui.diffuseC->setAutoFillBackground(true);
				QPalette palette2;
				palette2.setColor(ui.dif->backgroundRole(), QColor::fromRgbF(dif.r(),dif.g(),dif.b(),dif.a()));
				ui.dif->setPalette(palette2);
				ui.dif->setAutoFillBackground(true);
			}
			{
				QPalette palette;
				palette.setColor(QPalette::ButtonText, QColor::fromRgbF(spe.r(),spe.g(),spe.b(),spe.a()));
				ui.specularC->setPalette(palette);
				ui.specularC->setAutoFillBackground(true);
				QPalette palette2;
				palette2.setColor(ui.dif->backgroundRole(), QColor::fromRgbF(spe.r(),spe.g(),spe.b(),spe.a()));
				ui.spc->setPalette(palette2);
				ui.spc->setAutoFillBackground(true);
			}
		}

		{
			Etoile::Vec3f scale = current->getScale();
			ui.scaleX->setValue(scale.x());
			ui.scaleY->setValue(scale.y());
			ui.scaleZ->setValue(scale.z());

			Etoile::Quaternionf q = current->getRotation();
			Etoile::Vec3f axis = q.axis();
			ui.rotateX->setValue(axis.x());
			ui.rotateY->setValue(axis.y());
			ui.rotateZ->setValue(axis.z());
			ui.rotateAngle->setValue(q.angle());

			Etoile::Vec3f trans = current->getTranslation();
			ui.translateX->setValue(trans.x());
			ui.translateY->setValue(trans.y());
			ui.translateZ->setValue(trans.z());
		}
	
	}
}

Etoile::SubMesh* MeshEditing::getCurrentSubMesh()
{
	int idx = ui.submesh->currentIndex();
	if(_pMesh == NULL) return NULL;
	Etoile::SubMesh* current = NULL;
	const std::vector<Etoile::SubMesh*>& submeshlist = _pMesh->getSubMeshList();
	if(idx < submeshlist.size() && idx > -1)
	{
		current = submeshlist[idx];
		return current;
	}
}

void MeshEditing::setDiffuseColor()
{
	Etoile::SubMesh* sub = getCurrentSubMesh();
	if(sub == NULL) return;
	Etoile::Material* m = sub->getMaterial();
	if(m == NULL) return;
	const Etoile::Vec4f& dif = m->getDiffuse();
	QColor color = QColorDialog::getColor(QColor::fromRgbF(dif.r(),dif.g(),dif.b(),dif.a()), this);
	m->setDiffuse(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	QPalette palette;
	palette.setColor(QPalette::ButtonText, color);
	ui.diffuseC->setPalette(palette);
	ui.diffuseC->setAutoFillBackground(true);
	QPalette palette2;
	palette2.setColor(ui.dif->backgroundRole(), color);
	ui.dif->setPalette(palette2);
	ui.dif->setAutoFillBackground(true);
}

void MeshEditing::setAmbientColor()
{
	Etoile::SubMesh* sub = getCurrentSubMesh();
	if(sub == NULL) return;
	Etoile::Material* m = sub->getMaterial();
	if(m == NULL) return;
	const Etoile::Vec4f& amb = m->getAmbient();
	QColor color = QColorDialog::getColor(QColor::fromRgbF(amb.r(),amb.g(),amb.b(),amb.a()), this);
	m->setAmbient(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	QPalette palette;
	palette.setColor(QPalette::ButtonText, color);
	ui.ambientC->setPalette(palette);
	ui.ambientC->setAutoFillBackground(true);
	QPalette palette2;
	palette2.setColor(ui.amb->backgroundRole(), color);
	ui.amb->setPalette(palette2);
	ui.amb->setAutoFillBackground(true);
}

void MeshEditing::setSpecularColor()
{
	Etoile::SubMesh* sub = getCurrentSubMesh();
	if(sub == NULL) return;
	Etoile::Material* m = sub->getMaterial();
	if(m == NULL) return;
	const Etoile::Vec4f& spe = m->getSpecular();
	QColor color = QColorDialog::getColor(QColor::fromRgbF(spe.r(),spe.g(),spe.b(),spe.a()), this);
	m->setSpecular(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	QPalette palette;
	palette.setColor(QPalette::ButtonText, color);
	ui.specularC->setPalette(palette);
	ui.specularC->setAutoFillBackground(true);
	QPalette palette2;
	palette2.setColor(ui.spc->backgroundRole(), color);
	ui.spc->setPalette(palette2);
	ui.spc->setAutoFillBackground(true);
}

void MeshEditing::setTransform(double)
{
	Etoile::SubMesh* sub = getCurrentSubMesh();
	if(sub == NULL) return;
	Etoile::Vec3f scale(ui.scaleX->value(), ui.scaleY->value(), ui.scaleZ->value());
	Etoile::Quaternionf rotate(Etoile::Vec3f(ui.rotateX->value(),ui.rotateY->value(),ui.rotateZ->value()), ui.rotateAngle->value());
	Etoile::Vec3f translate(ui.translateX->value(), ui.translateY->value(), ui.translateZ->value());;
	sub->setTransform(scale, rotate, translate);
}

void MeshEditing::setMeshTransform(double)
{
	if(_pMesh == NULL) return;
	Etoile::Vec3f scale(ui.scaleX_2->value(), ui.scaleY_2->value(), ui.scaleZ_2->value());
	Etoile::Quaternionf rotate(Etoile::Vec3f(ui.rotateX_2->value(),ui.rotateY_2->value(),ui.rotateZ_2->value()), ui.rotateAngle_2->value());
	Etoile::Vec3f translate(ui.translateX_2->value(), ui.translateY_2->value(), ui.translateZ_2->value());;
	_pMesh->setTransform(scale, rotate, translate);
}