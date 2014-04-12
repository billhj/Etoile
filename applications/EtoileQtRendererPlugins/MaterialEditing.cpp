#include "MaterialEditing.h"
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

MaterialEditing::MaterialEditing(QWidget *parent)
	: QDockWidget(parent), _pMaterial(NULL)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	connect(ui.diffuseC, SIGNAL(clicked()), this, SLOT(setDiffuseColor()));
	connect(ui.specularC, SIGNAL(clicked()), this, SLOT(setSpecularColor()));
	connect(ui.ambientC, SIGNAL(clicked()), this, SLOT(setAmbientColor()));
}

MaterialEditing::~MaterialEditing()
{

}

void MaterialEditing::init(Etoile::Material* material)
{
	if(material == NULL) return;
	_pMaterial = material;
	updateParameters();
}

void MaterialEditing::updateParameters()
{
	if(_pMaterial == NULL) return;

	Etoile::Material* m = _pMaterial;
	if(m != NULL)
	{
		ui.id->setText(QString(m->getName().c_str()));
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


}

void MaterialEditing::setDiffuseColor()
{
	Etoile::Material* m = _pMaterial;
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

void MaterialEditing::setAmbientColor()
{
	Etoile::Material* m = _pMaterial;
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

void MaterialEditing::setSpecularColor()
{
	Etoile::Material* m = _pMaterial;
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