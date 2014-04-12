#include "SceneEditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include "util/File.h"
#include "QtTextureLoader.h"
#include "geometry/meshloader/OBJMeshLoader.h"
#include "geometry/meshloader/OpenMeshLoader.h"
#include "renderer/OpenGL/VBORenderMesh.h"
#include "renderer/OpenGL/ImmediateRenderMesh.h"
#include "renderer/OpenGL/GLRenderSceneSky.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

SceneEditor::SceneEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	this->setWindowTitle("SceneEditor");

	QStringList headers; 
	headers << "Scene" << "ID"; 
	ui.treeWidget->setHeaderLabels(headers); 
	_sceneitem.setText(0,tr("Scene"));
	ui.treeWidget->insertTopLevelItem(0,&_sceneitem);
	ui.treeWidget->setColumnWidth(0, 300);
	ui.treeWidget->setColumnWidth(1, 300);
	ui.treeWidget->setColumnWidth(2, 100);

	connect(ui.addSceneNode,SIGNAL(clicked()), this, SLOT(addSceneNode()));
	connect(ui.addSky,SIGNAL(clicked()), this, SLOT(addSky()));
	connect(ui.addEntity,SIGNAL(clicked()), this, SLOT(addSceneEntity()));
	connect(ui.addMesh,SIGNAL(clicked()), this, SLOT(addMesh()));
	connect(ui.addSkeleton,SIGNAL(clicked()), this, SLOT(addSkeleton()));
	connect(ui.addPlane,SIGNAL(clicked()), this, SLOT(addPlane()));
	connect(ui.transformation,SIGNAL(clicked()), this, SLOT(editTransformation()));
	connect(ui.material,SIGNAL(clicked()), this, SLOT(editMaterial()));

	initMenuBar();
}

SceneEditor::~SceneEditor()
{

}

void SceneEditor::initMenuBar()
{
	QMenu* scene = new QMenu("Scene", this);
	ui.menuBar->addMenu(scene);
	QAction* loadScene = new QAction("loadScene", this);
	scene->addAction(loadScene);
	connect(loadScene, SIGNAL(triggered()), this, SLOT(loadScene()));
	QAction* saveScene = new QAction("saveScene", this);
	scene->addAction(saveScene);
	connect(saveScene, SIGNAL(triggered()), this, SLOT(saveScene()));
}

void SceneEditor::loadScene()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::currentPath(), "*.xml");
	if (!fileName.isEmpty()) 
	{
		if(_pScene!=NULL)
		{
			loadScene(fileName.toStdString());
		}
	}
	else
	{
		QMessageBox::information(this, tr("loadScene"),
			tr("Cannot load %1.").arg(fileName));
	}
}

void SceneEditor::saveScene()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save GraphFile", QDir::currentPath(), "*.xml");
	if (!fileName.isEmpty()) 
	{
		if(_pScene!=NULL)
		{
			saveScene(fileName.toStdString());
		}
	}
	else
	{
		QMessageBox::information(this, tr("saveScene"),
			tr("Cannot load %1.").arg(fileName));
	}
}

void SceneEditor::init(Etoile::Scene* scene)
{
	if(scene == NULL) return;
	_pScene = scene;
	updateTreeView();
}

void SceneEditor::updateTreeView()
{
	if(_pScene == NULL) return;
	foreach(QTreeWidgetItem * item, _sceneitem.takeChildren())
	{
		_sceneitem.removeChild(item);
	}
	_sceneitem.setText(1,tr(_pScene->getStringID().c_str()));

	Etoile::SceneSky* sky = _pScene->getSkye();
	if(sky != NULL)
	{
		QTreeWidgetItem *skynode = new QTreeWidgetItem;
		skynode->setText(0,tr("Sky"));
		skynode->setText(1,tr(sky->getStringID().c_str()));
		skynode->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		_sceneitem.addChild(skynode);
	}

	for(unsigned int j = 0; j < _pScene->getSceneNodes().size(); ++j)
	{
		Etoile::SceneNode* node = _pScene->getSceneNodes()[j];
		_sceneitem.addChild(buildSceneNodeItem(node));
	}
	ui.treeWidget->setCurrentItem(&_sceneitem);
	ui.treeWidget->expandAll(); 
}

SceneNodeItem* SceneEditor::buildSceneNodeItem(Etoile::SceneNode* node)
{
	SceneNodeItem *scenenodeitem = new SceneNodeItem;
	scenenodeitem->setSceneNode(node);
	scenenodeitem->setText(0,tr("SceneNode"));
	scenenodeitem->setText(1,tr(node->getStringID().c_str()));
	scenenodeitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	for(unsigned int i = 0; i < node->getMovableObjects().size(); ++i)
	{
		Etoile::SceneEntity* entity = dynamic_cast<Etoile::SceneEntity*>(node->getMovableObjects()[i]);
		if(entity != NULL)
		{
			scenenodeitem->addChild(buildSceneEntityItem(entity));
		}

		Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(node->getMovableObjects()[i]);
		if(plane != NULL)
		{
			scenenodeitem->addChild(buildPlaneItem(plane));
		}
	}

	for(unsigned int i = 0; i < node->getChildrenNodes().size(); ++i)
	{
		scenenodeitem->addChild(buildSceneNodeItem(node->getChildrenNodes()[i]));
	}

	/*QTreeWidgetItem *transformationitem = new QTreeWidgetItem;
	transformationitem->setText(0,tr("tranformation"));
	//transformationitem->setText(1,tr("tranformation"));
	scenenodeitem->addChild(transformationitem);
	QTreeWidgetItem *addscenenodeitem = new QTreeWidgetItem;
	addscenenodeitem->setText(0,tr("addSceneNode"));
	//addscenenodeitem->setText(1,tr("addSceneNode"));
	scenenodeitem->addChild(addscenenodeitem);
	QTreeWidgetItem *addentityitem = new QTreeWidgetItem;
	addentityitem->setText(0,tr("addEntity"));
	//addentityitem->setText(1,tr("addEntity"));
	scenenodeitem->addChild(addentityitem);*/
	return scenenodeitem;
}

SceneEntityItem* SceneEditor::buildSceneEntityItem(Etoile::SceneEntity* entity)
{
	SceneEntityItem* entityitem = new SceneEntityItem();
	entityitem->setSceneEntity(entity);
	entityitem->setText(0,tr("SceneEntity"));
	entityitem->setText(1,tr(entity->getStringID().c_str()));
	entityitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

	Etoile::Mesh* mesh = entity->getMesh();
	if(mesh != NULL)
	{
		MeshItem* item = new MeshItem();
		item->setMesh(mesh);
		item->setText(0,tr("Mesh"));
		item->setText(1,tr(mesh->getName().c_str()));
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		entityitem->addChild(item);

		for(unsigned int i = 0; i < mesh->getSubMeshList().size(); ++i)
		{
			Etoile::SubMesh* submesh = mesh->getSubMeshList()[i];
			SubMeshItem* subitem = new SubMeshItem();
			subitem->setSubMesh(submesh);
			subitem->setText(0,tr("SubMesh"));
			subitem->setText(1,tr(submesh->getName().c_str()));
			item->addChild(subitem);
		}
	}

	Etoile::Skeleton* skeleton = entity->getSkeleton();
	if(skeleton != NULL)
	{
		SkeletonItem* item = new SkeletonItem();
		item->setSkeleton(skeleton);
		item->setText(0,tr("Skeleton"));
		item->setText(1,tr(skeleton->getName().c_str()));
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		entityitem->addChild(item);
	}

	return entityitem;
}

PlaneItem* SceneEditor::buildPlaneItem(Etoile::Plane* plane)
{
	PlaneItem* entityitem = new PlaneItem();
	entityitem->setPlane(plane);
	entityitem->setText(0,tr("Plane"));
	entityitem->setText(1,tr(plane->getStringID().c_str()));
	entityitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

	Etoile::Mesh* mesh = plane->getMesh();
	if(mesh != NULL)
	{
		MeshItem* item = new MeshItem();
		item->setMesh(mesh);
		item->setText(0,tr("Mesh"));
		item->setText(1,tr(mesh->getName().c_str()));
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		entityitem->addChild(item);

		for(unsigned int i = 0; i < mesh->getSubMeshList().size(); ++i)
		{
			Etoile::SubMesh* submesh = mesh->getSubMeshList()[i];
			SubMeshItem* subitem = new SubMeshItem();
			subitem->setSubMesh(submesh);
			subitem->setText(0,tr("SubMesh"));
			subitem->setText(1,tr(submesh->getName().c_str()));
			item->addChild(subitem);
		}
	}

	return entityitem;
}

void SceneEditor::addSceneNode()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneNodeItem* scenenodeitem = dynamic_cast<SceneNodeItem*>(item);
	if(scenenodeitem != NULL)
	{
		Etoile::SceneNode* node = new Etoile::SceneNode();
		scenenodeitem->getSceneNode()->addChildNode(node);
	}else
	{
		Etoile::SceneNode* node = new Etoile::SceneNode();
		_pScene->addSceneNode(node);
	}
	updateTreeView();
}

void SceneEditor::addSceneEntity()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneNodeItem* scenenodeitem = dynamic_cast<SceneNodeItem*>(item);
	if(scenenodeitem != NULL)
	{
		Etoile::SceneEntity* entity = new Etoile::SceneEntity();
		scenenodeitem->getSceneNode()->attachMovableObject(entity);
	}
	updateTreeView();
}

#include "SceneNodeEditing.h"
void SceneEditor::editTransformation()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneNodeItem* scenenodeitem = dynamic_cast<SceneNodeItem*>(item);
	if(scenenodeitem != NULL)
	{
		SceneNodeEditing* sne = new SceneNodeEditing(this);
		sne->init(scenenodeitem->getSceneNode());
		//sne->setFloating(true);
		this->addDockWidget(Qt::RightDockWidgetArea, sne);
		sne->setVisible(true);
	}
}

#include "MaterialEditing.h"
void SceneEditor::editMaterial()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SubMeshItem* subitem = dynamic_cast<SubMeshItem*>(item);
	if(subitem != NULL)
	{
		MaterialEditing* me = new MaterialEditing(this);
		me->init(subitem->getSubMesh()->getMaterial());
		//sne->setFloating(true);
		this->addDockWidget(Qt::RightDockWidgetArea, me);
		me->setVisible(true);
	}
}


void SceneEditor::addMesh()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneEntityItem* sceneentityitem = dynamic_cast<SceneEntityItem*>(item);
	if(sceneentityitem != NULL)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			Etoile::Mesh* mesh = openMesh(fileName.toStdString());
			sceneentityitem->getSceneEntity()->addMesh(mesh);
		}else{
			QMessageBox::information(this, tr("loadMesh"),
				tr("Cannot load %1.").arg(fileName));
		}
	}
	updateTreeView();
}

#include "renderer/OpenGL/GLTexture2D.h"

Etoile::Plane* SceneEditor::createPlane()
{
	Etoile::Mesh* mesh = new Etoile::AvancedGPUBasedVBORenderMesh("Plane");
	Etoile::Plane* plane = new Etoile::Plane();
	float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
	Etoile::GLTexture2D* t = new Etoile::GLTexture2D("emptyMap");
	t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
	float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
		1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
		0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
		1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
	Etoile::GLTexture2D* t2 = new Etoile::GLTexture2D("checkBoardMap");
	t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
	plane->setMesh(mesh);
	if(mesh != NULL)
	{
		for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
		{
			Etoile::SubMesh* submesh = mesh->getSubMeshList()[j];
			submesh->getMaterial()->setDiffuseTexture(t2);
		}
	}
	return plane;
}

void SceneEditor::addPlane()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneNodeItem* scenenodeitem = dynamic_cast<SceneNodeItem*>(item);
	if(scenenodeitem != NULL)
	{
		scenenodeitem->getSceneNode()->attachMovableObject(createPlane());
	}
	updateTreeView();
}

Etoile::SceneSky* SceneEditor::openSky(const std::string& filename)
{
	Etoile::QtTextureLoader textureloader;
	Etoile::Texture* t = textureloader.loadFromFile(filename);
	if(t != NULL)
	{
		Etoile::GLRenderSceneSky* sky = new Etoile::GLRenderSceneSky();
		sky->setSkyTexture(filename, t);
		return sky;
	}
	return NULL;
}

void SceneEditor::addSky()
{
	if(_pScene == NULL) return;
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"), QDir::currentPath());
	if (!fileName.isEmpty()) 
	{
		_pScene->setSky(openSky(fileName.toStdString()));
		updateTreeView();
	}else{
			QMessageBox::information(this, tr("loadMeshSky"),
				tr("Cannot load %1.").arg(fileName));
	}
}

Etoile::Mesh* SceneEditor::openMesh(const std::string& filename)
{
	if(filename.empty()) return NULL;
	std::string ext = Etoile::File::getFileExtension(filename);
	Etoile::RenderMesh* mesh;
	/*if(mode.compare("RenderMode_Immediate")==0)
	{
	mesh = new ImmediateRenderMesh(filename);
	}
	else if(mode.compare("RenderMode_GPUVBO")==0)
	{
	mesh = new GPUBasedVBORenderMesh(filename);
	}
	else if(mode.compare("RenderMode_AvancedGPUVBO")==0)
	{*/
	mesh = new Etoile::AvancedGPUBasedVBORenderMesh(filename);
	//}
	if(ext.compare("obj") == 0)
	{
		Etoile::OBJMeshLoader loader;
		Etoile::QtTextureLoader* textureloader = new Etoile::QtTextureLoader();
		loader.setTextureLoader(textureloader);
		loader.loadFromFile(filename, mesh);
	}
	else
	{
		Etoile::OpenMeshLoader loader;
		Etoile::QtTextureLoader* textureloader = new Etoile::QtTextureLoader();
		loader.setTextureLoader(textureloader);
		loader.loadFromFile(filename, mesh);
	}

	return mesh;
}


void SceneEditor::addSkeleton()
{
	if(_pScene == NULL) return;
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	SceneEntityItem* sceneentityitem = dynamic_cast<SceneEntityItem*>(item);
	if(sceneentityitem != NULL)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			Etoile::Skeleton* skeleton = openSkeleton(fileName.toStdString());
			sceneentityitem->getSceneEntity()->addSkeleton(skeleton);
		}else{
			QMessageBox::information(this, tr("loadSkeleton"),
				tr("Cannot load %1.").arg(fileName));
		}
	}
	updateTreeView();
}

#include "renderer/OpenGL/GLRenderSkeleton.h"
#include "animation/BVHLoader.h"
Etoile::Skeleton* SceneEditor::openSkeleton(const std::string& filename)
{
	Etoile::GLRenderSkeleton* skeleton = new Etoile::GLRenderSkeleton(filename);
	Etoile::BVHLoader _bvhloader;
	Etoile::KeyFrameSequence* sequence = new Etoile::KeyFrameSequence(filename);
	bool b = _bvhloader.loadFromBVHFile(filename, sequence, skeleton);
	if(!b)
		return NULL;
	return skeleton;
}


void SceneEditor::loadScene(const std::string& fileName)
{

	TiXmlDocument doc(fileName.c_str());
	if(!doc.LoadFile())
	{
		std::cout << "erreur while loading: " << fileName<<std::endl;
		std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
		QMessageBox::information(this, tr("OpenFile"),
			tr("Cannot load %1.").arg(fileName.c_str()));
	}
	TiXmlHandle hdl(&doc);

	TiXmlElement *elemRoot = hdl.FirstChildElement("Scene").Element();
	while (elemRoot)
	{
		TiXmlElement *nodeElement = elemRoot->FirstChildElement("SceneNode");
		loadSceneNode(nodeElement);

		TiXmlElement *skyElement = elemRoot->FirstChildElement("SceneSky");
		if(skyElement != NULL)
		{
			std::string file = skyElement->Attribute("file");
			_pScene->setSky(openSky(file));
		}

		elemRoot = elemRoot->NextSiblingElement("Scene"); // iteration
	}
	updateTreeView();
}

void SceneEditor::loadSceneNode(TiXmlElement *nodeElement)
{
	while (nodeElement)
	{
		TiXmlElement *subNodeElement = nodeElement->FirstChildElement("SceneNode");
		while (subNodeElement)
		{
			loadSceneNode(subNodeElement);
			subNodeElement = subNodeElement->NextSiblingElement("SceneNode"); // iteration	
		}

		Etoile::SceneNode* sn = new Etoile::SceneNode();
		_pScene->addSceneNode(sn);

		TiXmlElement *scaleE = nodeElement->FirstChildElement("scale");
		Etoile::Vec3f scale;
		{
			float x;
			if(TIXML_NO_ATTRIBUTE==scaleE->QueryFloatAttribute("x", &x))
			{
				x = 0;
			}
			float y;
			if(TIXML_NO_ATTRIBUTE==scaleE->QueryFloatAttribute("y", &y))
			{
				y = 0;
			}
			float z;
			if(TIXML_NO_ATTRIBUTE==scaleE->QueryFloatAttribute("z", &z))
			{
				z = 0;
			}
			scale.set(x,y,z);
		}

		Etoile::Quaternionf rot;
		TiXmlElement *rotationE = nodeElement->FirstChildElement("rotation");
		{
			float x;
			if(TIXML_NO_ATTRIBUTE==rotationE->QueryFloatAttribute("x", &x))
			{
				x = 0;
			}
			float y;
			if(TIXML_NO_ATTRIBUTE==rotationE->QueryFloatAttribute("y", &y))
			{
				y = 0;
			}
			float z;
			if(TIXML_NO_ATTRIBUTE==rotationE->QueryFloatAttribute("z", &z))
			{
				z = 0;
			}
			float w;
			if(TIXML_NO_ATTRIBUTE==rotationE->QueryFloatAttribute("w", &w))
			{
				w = 1;
			}
			rot.setValue(x,y,z,w);
		}

		Etoile::Vec3f tran;
		TiXmlElement *translationE = nodeElement->FirstChildElement("translation");
		{
			float x;
			if(TIXML_NO_ATTRIBUTE==translationE->QueryFloatAttribute("x", &x))
			{
				x = 0;
			}
			float y;
			if(TIXML_NO_ATTRIBUTE==translationE->QueryFloatAttribute("y", &y))
			{
				y = 0;
			}
			float z;
			if(TIXML_NO_ATTRIBUTE==translationE->QueryFloatAttribute("z", &z))
			{
				z = 0;
			}
			tran.set(x,y,z);
		}
		sn->setTransform(scale, rot, tran);


		TiXmlElement *entityElement = nodeElement->FirstChildElement("SceneEntity");
		while (entityElement)
		{
			Etoile::SceneEntity* entity = new Etoile::SceneEntity();
			sn->attachMovableObject(entity);
			TiXmlElement *meshElement = entityElement->FirstChildElement("Mesh");
			TiXmlElement *skeletonElement = entityElement->FirstChildElement("Skeleton");
			if(meshElement != NULL)
			{
				std::string file = meshElement->Attribute("file");
				entity->addMesh(openMesh(file));
			}

			if(skeletonElement != NULL)
			{
				std::string file = skeletonElement->Attribute("file");
				entity->addSkeleton(openSkeleton(file));
			}

			entityElement = entityElement->NextSiblingElement("SceneEntity"); // iteration	
		}


		TiXmlElement *planeElement = nodeElement->FirstChildElement("Plane");
		while (planeElement)
		{
			sn->attachMovableObject(createPlane());
			planeElement = planeElement->NextSiblingElement("Plane"); // iteration	
		}

		nodeElement = nodeElement->NextSiblingElement("SceneNode"); // iteration	
	}
}

void SceneEditor::saveScene(const std::string& filename)
{
	TiXmlDocument doc;
	if(_pScene != NULL)
	{

		TiXmlElement * root = new TiXmlElement( "Scene" );
		doc.LinkEndChild(root);
		std::vector<Etoile::SceneNode*>& nodes = _pScene->getSceneNodes();
		for(unsigned int i = 0; i < nodes.size(); ++i)
		{
			saveSceneNode(root, nodes[i]);
		}
		doc.SaveFile(filename.c_str());
	}
}

void SceneEditor::saveSceneNode(TiXmlElement *parentnodeElement, Etoile::SceneNode* node)
{
	TiXmlElement * nodeE = new TiXmlElement("SceneNode");
	TiXmlElement * nodeTran = new TiXmlElement("translation");
	Etoile::Vec3f tran = node->getTranslation();
	nodeTran->SetAttribute("x", tran.x());
	nodeTran->SetAttribute("y", tran.y());
	nodeTran->SetAttribute("z", tran.z());
	nodeE->LinkEndChild(nodeTran);
	TiXmlElement * nodeRot = new TiXmlElement("rotation");
	Etoile::Quaternionf rot = node->getRotation();
	nodeRot->SetAttribute("x", rot.x());
	nodeRot->SetAttribute("y", rot.y());
	nodeRot->SetAttribute("z", rot.z());
	nodeRot->SetAttribute("w", rot.w());
	nodeE->LinkEndChild(nodeRot);
	TiXmlElement * nodeScal = new TiXmlElement("scale");
	Etoile::Vec3f scal = node->getScale();
	nodeScal->SetAttribute("x", scal.x());
	nodeScal->SetAttribute("y", scal.y());
	nodeScal->SetAttribute("z", scal.z());
	nodeE->LinkEndChild(nodeScal);

	std::vector<Etoile::SceneNode*>& nodes = node->getChildrenNodes();
	for(unsigned int i = 0; i < nodes.size(); ++i)
	{
		saveSceneNode(nodeE, nodes[i]);
	}

	std::vector<Etoile::MovableObject*>& mo = node->getMovableObjects();
	for(unsigned int i = 0; i < mo.size(); ++i)
	{
		Etoile::MovableObject* movable = mo[i];
		Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(movable);
		if(plane!=NULL)
		{
			TiXmlElement * nodeP = new TiXmlElement("Plane");
			nodeE->LinkEndChild(nodeP);
		}

		Etoile::SceneEntity* entity = dynamic_cast<Etoile::SceneEntity*>(movable);
		if(entity!=NULL)
		{
			TiXmlElement * nodeEntity = new TiXmlElement("SceneEntity");
			nodeE->LinkEndChild(nodeEntity);

			Etoile::Mesh* mesh = entity->getMesh();
			if(mesh!=NULL)
			{
				TiXmlElement * nodeMesh = new TiXmlElement("Mesh");
				nodeMesh->SetAttribute("file", mesh->getName().c_str());
				nodeEntity->LinkEndChild(nodeMesh);
			}
			Etoile::Skeleton* sk = entity->getSkeleton();
			if(sk!=NULL)
			{
				TiXmlElement * nodeSK = new TiXmlElement("Skeleton");
				nodeSK->SetAttribute("file", sk->getName().c_str());
				nodeEntity->LinkEndChild(nodeSK);
			}
		}
	}

	parentnodeElement->LinkEndChild(nodeE);
}