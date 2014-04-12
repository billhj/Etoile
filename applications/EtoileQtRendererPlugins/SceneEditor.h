#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include <QMainWindow>
#include <QTreeWidget>
#include "ui_SceneEditor.h"
#include "geometry/SceneEntity.h"
#include "geometry/Scene.h"
#include "geometry/Primitive.h"
#include "tinyxml/tinyxml.h"

class SceneNodeItem : public QTreeWidgetItem 
{
public:
	SceneNodeItem(): QTreeWidgetItem(), _pnode(NULL){}
	void setSceneNode(Etoile::SceneNode* node){ _pnode = node;}
	Etoile::SceneNode* getSceneNode(){return _pnode;}
private:
	Etoile::SceneNode* _pnode;
};

class SceneEntityItem : public QTreeWidgetItem 
{
public:
	SceneEntityItem(): QTreeWidgetItem(), _pSceneEntity(NULL){}
	void setSceneEntity(Etoile::SceneEntity* entity){ _pSceneEntity = entity;}
	Etoile::SceneEntity* getSceneEntity(){return _pSceneEntity;}
private:
	Etoile::SceneEntity* _pSceneEntity;
};

class PlaneItem : public QTreeWidgetItem 
{
public:
	PlaneItem(): QTreeWidgetItem(), _pPlane(NULL){}
	void setPlane(Etoile::Plane* plane){ _pPlane = plane;}
	Etoile::Plane* getPlane(){return _pPlane;}
private:
	Etoile::Plane* _pPlane;
};

class MeshItem : public QTreeWidgetItem 
{
public:
	MeshItem(): QTreeWidgetItem(), _pMesh(NULL){}
	void setMesh(Etoile::Mesh* mesh){ _pMesh = mesh;}
	Etoile::Mesh* getMesh(){return _pMesh;}
private:
	Etoile::Mesh* _pMesh;
};

class SubMeshItem : public QTreeWidgetItem 
{
public:
	SubMeshItem(): QTreeWidgetItem(), _pMesh(NULL){}
	void setSubMesh(Etoile::SubMesh* submesh){ _pMesh = submesh;}
	Etoile::SubMesh* getSubMesh(){return _pMesh;}
private:
	Etoile::SubMesh* _pMesh;
};

class SkeletonItem : public QTreeWidgetItem 
{
public:
	SkeletonItem(): QTreeWidgetItem(), _pSkeleton(NULL){}
	void setSkeleton(Etoile::Skeleton* sk){ _pSkeleton = sk;}
	Etoile::Skeleton* getSkeleton(){return _pSkeleton;}
private:
	Etoile::Skeleton* _pSkeleton;
};

class SceneEditor : public QMainWindow
{
	Q_OBJECT
public:
	SceneEditor(QWidget *parent = 0);
	~SceneEditor();
	void initMenuBar();
	void init(Etoile::Scene* scene);
	//void setCommonGpuProgram(Etoile::GpuProgram* pGpuCommon){_pGpuCommon = pGpuCommon;}
	void updateTreeView();
	SceneNodeItem* buildSceneNodeItem(Etoile::SceneNode* node);
	SceneEntityItem* buildSceneEntityItem(Etoile::SceneEntity* entity);
	PlaneItem* buildPlaneItem(Etoile::Plane* plane);
	
	Etoile::Mesh* openMesh(const std::string& filename);
	Etoile::Skeleton* openSkeleton(const std::string& filename);
	Etoile::Plane* createPlane();
	Etoile::SceneSky* openSky(const std::string& filename);
	public slots:
		void loadScene();
		void saveScene();
		void addSky();
		void addSceneEntity();
		void addSceneNode();
		void addMesh();
		void addSkeleton();
		void editTransformation();
		void editMaterial();
		void addPlane();
private:
	void loadScene(const std::string& filename);
	void loadSceneNode(TiXmlElement *nodeElement);
	void saveScene(const std::string& filename);
	void saveSceneNode(TiXmlElement *parentnodeElement, Etoile::SceneNode* node);
	Ui::SceneEditor ui;
	QTreeWidgetItem _sceneitem;
	Etoile::Scene* _pScene;
	//Etoile::GpuProgram * _pGpuCommon;
};




#endif // SCENEEDITOR_H
