#include "SceneNodeEditing.h"
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

SceneNodeEditing::SceneNodeEditing(QWidget *parent)
	: QDockWidget(parent), _pSceneNode(NULL)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	
	connect(ui.rotateX_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateY_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateZ_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.rotateAngle_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateX_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateY_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.translateZ_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleX_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleY_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));
	connect(ui.scaleZ_2,SIGNAL(valueChanged(double)), this, SLOT(setTransform(double)));

	connect(ui.update, SIGNAL(clicked()), this, SLOT(reflash()));
}

SceneNodeEditing::~SceneNodeEditing()
{

}


void SceneNodeEditing::init(Etoile::SceneNode* scenenode)
{
	
	if(scenenode == NULL) return;
	_pSceneNode = scenenode;
	ui.id->setText(QString(scenenode->getStringID().c_str()));
	Etoile::Vec3f scale = _pSceneNode->getScale();
	ui.scaleX_2->setValue(scale.x());
	ui.scaleY_2->setValue(scale.y());
	ui.scaleZ_2->setValue(scale.z());

	Etoile::Quaternionf q = _pSceneNode->getRotation();
	Etoile::Vec3f axis = q.axis();
	ui.rotateX_2->setValue(axis.x());
	ui.rotateY_2->setValue(axis.y());
	ui.rotateZ_2->setValue(axis.z());
	ui.rotateAngle_2->setValue(q.angle());

	Etoile::Vec3f trans = _pSceneNode->getTranslation();
	ui.translateX_2->setValue(trans.x());
	ui.translateY_2->setValue(trans.y());
	ui.translateZ_2->setValue(trans.z());
}

void SceneNodeEditing::reflash()
{
	if(_pSceneNode == NULL) return;
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

	Etoile::Vec3f scale = _pSceneNode->getScale();
	ui.scaleX_2->setValue(scale.x());
	ui.scaleY_2->setValue(scale.y());
	ui.scaleZ_2->setValue(scale.z());

	Etoile::Quaternionf q = _pSceneNode->getRotation();
	Etoile::Vec3f axis = q.axis();
	ui.rotateX_2->setValue(axis.x());
	ui.rotateY_2->setValue(axis.y());
	ui.rotateZ_2->setValue(axis.z());
	ui.rotateAngle_2->setValue(q.angle());

	Etoile::Vec3f trans = _pSceneNode->getTranslation();
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

void SceneNodeEditing::setTransform(double)
{
	if(_pSceneNode == NULL) return;
	Etoile::Vec3f scale(ui.scaleX_2->value(), ui.scaleY_2->value(), ui.scaleZ_2->value());
	Etoile::Quaternionf rotate(Etoile::Vec3f(ui.rotateX_2->value(),ui.rotateY_2->value(),ui.rotateZ_2->value()), ui.rotateAngle_2->value());
	Etoile::Vec3f translate(ui.translateX_2->value(), ui.translateY_2->value(), ui.translateZ_2->value());;
	_pSceneNode->setTransform(scale, rotate, translate);
}