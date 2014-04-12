/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLSceneRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileGLSceneRenderPassPlugin.h"
#include "util/File.h"
#include "QtTextureLoader.h"
#include "tinyxml/tinyxml.h"
#include <QFileDialog>
#include <QMessageBox>

Etoile::EPlugin* loadEtoileGLSceneRenderPassPlugin()
{
	return new Etoile::EtoileGLSceneRenderPassPlugin("CommonSceneRenderPass");
}

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	EtoileSceneInputSocket::EtoileSceneInputSocket(const std::string name) : SceneInputSocket(name)
	{
	}


	void EtoileSceneInputSocket::perform(Scene* signal)
	{
		if(signal == NULL) return;
		GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
		//retrieve(signal);
		plugin->addScene(signal);
	}

	void EtoileSceneInputSocket::retrieve(Scene* signal)
	{
		GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		unsigned int usize = units.size();
		for(unsigned int i = 0; i < usize; ++i)
		{
			GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
			if(munit != NULL)
			{
				Scene* m = munit->getScene();
				if(m->getID() == signal->getID())
				{
					units.erase(units.begin() + i);
					--usize;
					--i;
				}
			}
		}
	}


	EtoileScenePassTextureInputSocket::EtoileScenePassTextureInputSocket(const std::string& name):TextureInputSocket(name)
	{
	}

	void EtoileScenePassTextureInputSocket::perform(Texture* signal)
	{
		if(signal == NULL) return;
		GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		for(unsigned int i = 0; i < units.size(); ++i)
		{
			GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
			if(munit != NULL)
			{
				Scene* scene = munit->getScene();
				for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
				{
					SceneNode* node = scene->getSceneNodes()[j];
					for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
					{
						MovableObject* obj = node->getMovableObjects()[k];
						SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
						if(entity != NULL)
						{
							Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
							if(mesh != NULL)
							{
								for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
								{
									SubMesh* submesh = mesh->getSubMeshList()[l];
									Material* mt = submesh->getMaterial();
									mt->setTexture(this->getName(), signal);
#if defined(_DEBUG) || defined(DEBUG)
									std::cout<<"class EtoileScenePassTextureInputSocket: set texture signal: "<< this->getName() <<"  " << signal->getName() <<std::endl;
#endif
								}
							}
						}
					}
				}
			}
		}
	}

	void EtoileScenePassTextureInputSocket::retrieve(Texture* signal)
	{
		if(signal == NULL) return;
		GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		for(unsigned int i = 0; i < units.size(); ++i)
		{
			GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
			if(munit != NULL)
			{
				Scene* scene = munit->getScene();
				for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
				{
					SceneNode* node = scene->getSceneNodes()[j];
					for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
					{
						MovableObject* obj = node->getMovableObjects()[k];
						SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
						if(entity != NULL)
						{
							Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
							if(mesh != NULL)
							{
								for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
								{
									SubMesh* submesh = mesh->getSubMeshList()[l];
									Material* mt = submesh->getMaterial();
									mt->removeTexture(this->getName());
#if defined(_DEBUG) || defined(DEBUG)
									std::cout<<"class EtoileScenePassTextureInputSocket: delete texture signal: "<< this->getName() <<"  " << signal->getName() <<std::endl;
#endif
								}
							}
						}
					}
				}
			}
		}
	}

	EtoileGLSceneRenderPassPlugin::EtoileGLSceneRenderPassPlugin(const std::string& name): EPlugin(), GLSceneRenderPass(name), _pview(NULL), _pproj(NULL), _pviewproj(NULL),_psceneinput(NULL)
	{
		this->getType()._description = "DrawScene";
		this->getType()._color._r = 80;
		this->getType()._color._g = 240;
		this->getType()._color._b = 250;
		this->getType()._color._a = 240;
		widget = new ParameterInputSocketsWidget();
		widget->setVisible(false);
	
		_pview = new EtoileScenePassViewMatrix4fInputSocket;
		_pproj = new EtoileScenePassProjectionMatrix4fInputSocket;
		_pviewproj = new EtoileScenePassViewProjectionMatrix4fInputSocket;
		this->addInputSocket(_pview);
		this->addInputSocket(_pproj);
		this->addInputSocket(_pviewproj);
		_psceneinput = new EtoileSceneInputSocket();
		this->addInputSocket(_psceneinput);

		/*QString fileName = QFileDialog::getOpenFileName(0, QString("select SceneRenderPass"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			loadFile(fileName.toStdString());
		}else{
			QMessageBox::information(0, QString("OpenFile"),
				QString("Cannot load %1.").arg(fileName));
		}*/
	}

	EtoileGLSceneRenderPassPlugin::~EtoileGLSceneRenderPassPlugin()
	{

	}


	void EtoileGLSceneRenderPassPlugin::init()
	{
		
	}

	void EtoileGLSceneRenderPassPlugin::apply()
	{
		widget->setVisible(true);
		widget->raise();
	}

	void EtoileGLSceneRenderPassPlugin::release()
	{
	
	}

	bool EtoileGLSceneRenderPassPlugin::loadFile(const std::string& filename)
	{
		if(filename.empty()) return false;
		std::string ext = File::getFileExtension(filename);
		std::string path = File::getFilePath(filename);
		TiXmlDocument doc(filename.c_str());
		if(!doc.LoadFile())
		{
			std::cout << "erreur while loading: " << filename<<std::endl;
			std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
			return false;
		}
		this->getOutputSockets().clear();
		this->getInputSockets().clear();

		if(_pview != NULL)
		{
			this->addInputSocket(_pview);
		}
		if(_pproj != NULL)
		{
			this->addInputSocket(_pproj);
		}
		if(_pviewproj != NULL)
		{
			this->addInputSocket(_pviewproj);
		}

		_psceneinput = new EtoileSceneInputSocket();
		if(_psceneinput != NULL){
			this->addInputSocket(_psceneinput);
		}

		AABBWidget* aabbw = new AABBWidget();
		aabbw->bindPass(this);
		widget->addWidget(aabbw);
		TiXmlHandle hdl(&doc);

		TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
		while (elemRoot)
		{
			TiXmlElement *passElement = elemRoot->FirstChildElement("ScenePass");
			while (passElement)
			{
				std::string name = passElement->Attribute("name");
				this->getType()._description = name;

				int x;
				if(TIXML_NO_ATTRIBUTE!=passElement->QueryIntAttribute("x", &x))
				{
					this->_x = x;
				}
				int y;
				if(TIXML_NO_ATTRIBUTE!=passElement->QueryIntAttribute("y", &y))
				{
					this->_y = y;
				}
				int w;
				if(TIXML_NO_ATTRIBUTE!=passElement->QueryIntAttribute("w", &w))
				{
					this->_w = w;
				}
				int h;
				if(TIXML_NO_ATTRIBUTE!=passElement->QueryIntAttribute("h", &h))
				{
					this->_h = h;
				}
				initFBO();
				TiXmlElement *gpuElement = passElement->FirstChildElement("GPU");
				while (gpuElement)
				{
					const char* name = gpuElement->Attribute("name");
					std::map<ShaderType, std::string> files;
					TiXmlElement *vertexElement = gpuElement->FirstChildElement("vertex");
					if(vertexElement != NULL)
					{
						const char* file = vertexElement->Attribute("file");
						files[ShaderType::SHADERTYPE_VERTEX] = path + file;
					}
					TiXmlElement *fragElement = gpuElement->FirstChildElement("fragment");
					if(fragElement != NULL)
					{
						const char* file = fragElement->Attribute("file");
						files[SHADERTYPE_FRAGMENT] = path + file;
					}
					TiXmlElement *geometryElement = gpuElement->FirstChildElement("geometry");
					if(geometryElement != NULL)
					{
						const char* file = geometryElement->Attribute("file");
						files[SHADERTYPE_GEOMETRY] = path + file;
					}
					TiXmlElement *computeElement = gpuElement->FirstChildElement("compute");
					if(computeElement != NULL)
					{
						const char* file = computeElement->Attribute("file");
						files[SHADERTYPE_COMPUTE] = path + file;
					}
					TiXmlElement *tessCtrElement = gpuElement->FirstChildElement("tessctrl");
					if(tessCtrElement != NULL)
					{
						const char* file = tessCtrElement->Attribute("file");
						files[SHADERTYPE_TESSELLATION_CTRL] = path + file;
					}
					TiXmlElement *tessEvaElement = gpuElement->FirstChildElement("tesseval");
					if(tessEvaElement != NULL)
					{
						const char* file = tessEvaElement->Attribute("file");
						files[SHADERTYPE_TESSELLATION_EVAL] = path + file;
					}
					GLSLGpuProgram* glsl = new GLSLGpuProgram(name);
					glsl->loadShaderFiles(files);
					this->setCommonGpuProgram(glsl);
					//_editor->setCommonGpuProgram(glsl);
					GpuCompileWidget* gwidget = new GpuCompileWidget();
					gwidget->bindPass(this);
					widget->addWidget(gwidget);
					gpuElement = gpuElement->NextSiblingElement("GPU"); // iteration
				}

				TiXmlElement *outputsElement = passElement->FirstChildElement("Output");
				while (outputsElement)
				{
					std::string type = outputsElement->Attribute("type");
					std::string name = outputsElement->Attribute("name");
					if(type.compare("texture2d")==0)
					{
						TextureRenderTarget* tt = new TextureRenderTarget(name);
						GLTexture2D* t = new GLTexture2D(name);
						t->create(w,h,1);
						tt->set(t);
						this->addOutputSocket(tt);
					}

					outputsElement = outputsElement->NextSiblingElement("Output"); // iteration
				}

				TiXmlElement *inputsElement = passElement->FirstChildElement("Input");
				while (inputsElement)
				{
					std::string type = inputsElement->Attribute("type");
					std::string name = inputsElement->Attribute("name");
					if(type.compare("texture2d")==0)
					{
						EtoileScenePassTextureInputSocket* in = new EtoileScenePassTextureInputSocket(name);
						this->addInputSocket(in);
					}
					else if(type.compare("float")==0)
					{
						EtoileScenePassFloatInputSocket* in = new EtoileScenePassFloatInputSocket(name);
						in->setNode(this);
						//this->addInputSocket(in);
						FloatInputSocketWidget* socketwidget = new FloatInputSocketWidget(widget);
						float value;
						if(TIXML_NO_ATTRIBUTE!=inputsElement->QueryFloatAttribute("value", &value))
						{
							in->perform(&value);
						}
						socketwidget->bindParameter(in, value);
						widget->addWidget(socketwidget);
					}
					else if(type.compare("int")==0)
					{
						EtoileScenePassIntInputSocket* in = new EtoileScenePassIntInputSocket(name);
						in->setNode(this);
						//this->addInputSocket(in);
						IntInputSocketWidget* socketwidget = new IntInputSocketWidget(widget);
						int value = 0;
						if(TIXML_NO_ATTRIBUTE!=inputsElement->QueryIntAttribute("value", &value))
						{
							in->perform(&value);
						}
						socketwidget->bindParameter(in, value);
						widget->addWidget(socketwidget);
					}

					inputsElement = inputsElement->NextSiblingElement("Input"); // iteration
				}


				passElement = passElement->NextSiblingElement("ScenePass"); // iteration
			}

			elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
		}

		return true;
	}

}