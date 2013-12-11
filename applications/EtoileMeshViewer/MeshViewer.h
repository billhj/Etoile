/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.h
* @brief 
* @date 1/2/2011
*/
#pragma once

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include <QGLViewer/qglviewer.h>
#include <QImage>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "geometry/Mesh.h"
#include "geometry/MeshLoader/OBJMeshLoader.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/UniformBufferObject.h"
#include "renderer/OpenGL/ImmediateMeshRenderUnit.h"
#include "QTTextureLoader.h"
#include "renderer/OpenGL/GLMeshRenderPass.h"
#include "renderer/OpenGL/GLQuadRenderPass.h"
#include "renderer/OpenGL/LightController.h"
#include "GpuProgramInit.h"
//#include "RendererInit.h"
#include "LightInit.h"
#include "Core.h"
#include "util/SamplesGenerator.h"
#include "math/Vectors.h"
#include "util/Dir.h"
#include "util/File.h"
using namespace Etoile;
class MeshViewer : public QGLViewer
{
	Q_OBJECT
public:
	MeshViewer():QGLViewer(), _pMesh(NULL), rp(NULL)
	{
	}

	~MeshViewer()
	{
		clear();
	}

	void clear()
	{
		
	}

	void init()
	{
		glInfo* info = glInfo::getInstance();
		info->initAllOpenGL();


		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glFrontFace(GL_CW);
		glClearColor(0.0, 0.0, 0.0, 0.99);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);
		//this->setSceneCenter(qglviewer::Vec(0,0,0));
		//this->setSceneRadius(0.1);
		//glDisable(GL_COLOR_MATERIAL);
		//glEnable(GL_TEXTURE);
		//_pTransform = new UniformBufferObject(4 * sizeof(Matrix4f), GL_DYNAMIC_DRAW, 0);
		QString str = QDir::currentPath();
		const std::string dir = Dir::getCurrentDirectory();
		//_initG.init(str.toStdString() + "/shader/gpu.xml");
		_initL.init(str.toStdString() + "/shader/light.xml");
	}

	void open(const std::string& name)
	{
		OBJMeshLoader objloader;
		_textureloader = new QTTextureLoader();
		objloader.setTextureLoader(_textureloader);
		std::cout<< "loadMesh" << std::endl;
		QTime qtime;
		qtime.start();

		Mesh * mesh= new Mesh("mesh");
		if(Etoile::File::getFileExtension(name).compare("obj")!=0) return;
		bool e = objloader.loadFromFile(name, mesh);
		if(e)
		{
			_pMesh = mesh;
			std::map<std::string, std::string> txtPath = objloader.getTexturePathMap();
			std::vector<Material*>& mats = objloader.getMaterials();

			//loadTextures(txtPath);
			std::cout<<"loading time: "<<qtime.elapsed()<<" msc"<<std::endl;

			std::vector<SubMesh*> submeshes = _pMesh->getSubMeshList();
			for(unsigned int i = 0; i < submeshes.size(); ++i)
			{
				SubMesh* sub = submeshes[i];
				Material* m = sub->getMaterial();
				//m->setGpuProgram("drawmesh");
			}

			setBasicRenderer();
		}
	}

	void readMeshFile()
	{
		QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
		open(name.toStdString());
		//setGPUProgramRenderer();
	}

	

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		updateLight();
		
		if(rp != NULL)
		{
		//updateMatrix();
			rp->setViewPort(0, 0,this->width(), this->height());
			rp->draw();
		}
	}

	void loadTextures(std::map<std::string, std::string>& txtPath)
	{
		std::map<std::string, std::string>::iterator itor;
		for(itor = txtPath.begin(); itor != txtPath.end(); ++itor)
		{
			std::string textureName = itor->first;
			std::string texturePath = itor->second;

			Image image;
			bool b_image = ReadImage::loadImageFromFile(texturePath, image);

			if(b_image != true)
			{
				std::cout<<"can not load texture : "<<textureName<<std::endl;
				//assert(!qimage.isNull());
			}
			else
			{
				GLTexture2D* t = new GLTexture2D(textureName);
				t->create(image.getWidth(), image.getHeight(),1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT ,(float*)&image.getData()[0], false);
			}

		}
		float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
		float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
		GLTexture2D* t = new GLTexture2D("emptyMap");
		t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
		GLTexture2D* t2 = new GLTexture2D("checkBoardMap");
		t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);

	}

	void updateLight()
	{
		std::vector<Light*> lights = _initL.getLights();
		for(unsigned int i = 0; i < lights.size(); ++i)
		{
			Light* light = (Light*)lights[i];
			LightController controller;
			controller.bindLight(light);
			controller.use();
		}
	}

	void updateMatrix()
	{
		Matrix4f transform[4];
		glMatrixMode (GL_PROJECTION);
		glGetFloatv(GL_PROJECTION_MATRIX , &(transform[0][0][0]));
		printOpenGLError ();
		glMatrixMode(GL_MODELVIEW);
		glGetFloatv(GL_MODELVIEW_MATRIX ,&(transform[1][0][0]));
		printOpenGLError ();
		transform[2] = transform[0] * transform[1];
		transform[3] = transform[1];
		transform[3].inverse();
		transform[3].transpose();
		_pTransform->writeData(4 * sizeof(Matrix4f), &transform[0][0]);
		std::vector<GLSLGpuProgram*>& _gpuprograms = _initG.getGpuPrograms();
		for(unsigned int i = 0; i < _gpuprograms.size(); ++i)
		{
			GLSLGpuProgram* gpupragram = ( GLSLGpuProgram*)_gpuprograms[i];
			if(gpupragram != NULL)
			{
				gpupragram->setUniformBufferObject("In_Transform", _pTransform);
				gpupragram->setUniformVariable("In_ViewMatrix", transform[1]);
				gpupragram->setUniformVariable("In_ProjectionMatrix", transform[0]);
				gpupragram->setUniformVariable("In_ViewProjectionMatrix", transform[2]);
				gpupragram->setUniformVariable("In_NormalMatrix", transform[3]);
				//gpuprogamcontroller->setGpuProgramParameter("viewDirection", Vec3f (this->camera()->viewDirection()[0],this->camera()->viewDirection()[1],this->camera()->viewDirection()[2] ));
			}
		}
	}

	void generateSampling()
	{
		HammersleySamplesGenerator* sg = new HammersleySamplesGenerator("Hammersley");
		sg->setSize(16);
		sg->setPatternSize(4);
		sg->generateSamples();
		std::vector<SamplesGenerator::Sample> samples = sg->getSamples();
	
		std::vector<GLSLGpuProgram*>& _gpuprograms = _initG.getGpuPrograms();
		for(unsigned int i = 0; i < _gpuprograms.size(); ++i)
		{
			GLSLGpuProgram* gpupragram = ( GLSLGpuProgram*)_gpuprograms[i];
			if(gpupragram != NULL)
			{
				gpupragram->setUniformVariable("PatternSamples", VecConverter::convertFromDoubleToFloat(samples));	
				gpupragram->setUniformVariable("SamplesNumber", int(16));
				gpupragram->setUniformVariable("PatternSize", int(4));
			}
		}
	}

	void setMesh(Mesh* mesh)
	{
		if(mesh != NULL)
		{
			if(_pMesh != NULL)
			{
				delete rp;
				delete _pMesh;
			}
			_pMesh = mesh;
			setBasicRenderer();
		}
	}

	public slots:
		void setBasicRenderer()
		{
			if(_pMesh != NULL){
				rp = new GLMeshRenderPass("empty");
				ImmediateMeshRenderUnit* _organizer = new ImmediateMeshRenderUnit("simple");
				_organizer->setMesh(_pMesh);
				rp->addRenderUnit(_organizer);
			}
		}

		/*void setBasicGPUProgramRenderer()
		{
			_pRenderman->clear();
			GLMeshRenderPass* pass = new GLMeshRenderPass("drawmesh");
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				pass->addMesh(_pMesh);
			}
			_pRenderman->addRenderPass(pass);
		}

		void setGPUProgramRenderer()
		{

			_pRenderman->clear();
			GLMeshRenderPass* pass = new GLMeshRenderPass("drawmesh");
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				pass->addMesh(_pMesh);
			}
			_pRenderman->addRenderPass(pass);

			GLMultiRenderTarget* target = new GLMultiRenderTarget("drawmesh", 800, 600);
			pass->setRenderTarget(target);

			GLTexture2D* t = new GLTexture2D("normalMap");
			t->create(800, 600, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , 0, false);
			target->addTextureTarget(t->getName());
			_pRenderman->getTextureManager()->addTexture(t);
			GLTexture2D* t2 = new GLTexture2D("positionMap");
			t2->create(800, 600, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , 0, false);
			target->addTextureTarget(t2->getName());
			_pRenderman->getTextureManager()->addTexture(t2);

			QuadRenderPass* quadpass = new QuadRenderPass("output");
			Material* mt= new Material("output"); 
			mt->setGpuProgram("output");
			mt->setTexture("output", "normalMap");
			quadpass->setMaterial(mt->getName());
			_pRenderman->getMaterialManager()->addMaterial(mt);
			_pRenderman->addRenderPass(quadpass);

		}*/

		void setPassFileRenderer()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("renderer (*.xml)"));
			std::cout<< "loadRenderer" << std::endl;
			if(!name.isEmpty())
			{
				//_initR.init(name.toStdString());
			}
		}

		void loadGpuPro()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("gpuprogram (*.xml)"));
			std::cout<< "loadGPUpro" << std::endl;
			if(!name.isEmpty())
			{
				_initG.init(name.toStdString());
				generateSampling();
			}
		}

		void reloadGpuPro()
		{
			_initG.reInit();
			generateSampling();
		}

		void reloadPassRenderer()
		{
			//_initR.reInit();
		}


		void loadLight()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("light (*.xml)"));
			std::cout<< "loadLight" << std::endl;
			if(!name.isEmpty())
			{
				_initL.init(name.toStdString());
			}
		}

		void reloadLight()
		{
			_initL.reInit();
		}

private:
	UniformBufferObject* _pTransform;
	std::map<std::string, GLuint> _textureMap;
	//RendererInit _initR;
	GpuProgramInit _initG;
	LightInit _initL;
	Mesh* _pMesh;
	GLMeshRenderPass* rp;
	QTTextureLoader* _textureloader;
};
