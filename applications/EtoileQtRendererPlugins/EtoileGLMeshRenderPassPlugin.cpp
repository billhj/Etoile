/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLMeshRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileGLMeshRenderPassPlugin.h"
#include "util/File.h"
#include "QtTextureLoader.h"
#include "tinyxml/tinyxml.h"
#include <QFileDialog>
#include <QMessageBox>

Etoile::EPlugin* loadEtoileGLMeshRenderPassPlugin()
{
	return new Etoile::EtoileGLMeshRenderPassPlugin("CommonMeshRenderPass");
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

	EtoileMeshInputSocket::EtoileMeshInputSocket(const std::string name) : MeshInputSocket(name)
	{
	}


	void EtoileMeshInputSocket::perform(Mesh* signal)
	{
		if(signal == NULL) return;
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		plugin->addMesh(signal);
	}

	void EtoileMeshInputSocket::retrieve(Mesh* signal)
	{
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		unsigned int usize = units.size();
		for(unsigned int i = 0; i < usize; ++i)
		{
			RenderUnit* unit = units[i];
			MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
			if(munit != NULL)
			{
				Mesh* m = munit->getMesh();
				if(m->getDeviceID() == signal->getDeviceID())
				{
					units.erase(units.begin() + i);
					--usize;
					--i;
				}
			}
		}
	}

	EtoileMeshRenderUnitInputSocket::EtoileMeshRenderUnitInputSocket(const std::string name) : TemplateInputSocket<MeshRenderUnit>(INPUT_MUL)
	{
		this->initType("SOCK_MESH_RENDER_UNIT");
		setName(name);
	}

	void EtoileMeshRenderUnitInputSocket::perform(MeshRenderUnit* signal)
	{
		if(signal == NULL) return;
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		plugin->addRenderUnit(signal);
	}

	void EtoileMeshRenderUnitInputSocket::retrieve(MeshRenderUnit* signal)
	{
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		unsigned int usize = units.size();
		for(unsigned int i = 0; i < usize; ++i)
		{
			RenderUnit* unit = units[i];
			GLMeshRenderUnit* munit = (GLMeshRenderUnit*)(unit);
			if(munit->getMesh()->getDeviceID() == signal->getMesh()->getDeviceID())
			{
				units.erase(units.begin() + i);
				--usize;
				--i;
			}
		}
	}

	EtoileMeshPassTextureInputSocket::EtoileMeshPassTextureInputSocket(const std::string& name):TextureInputSocket(name)
	{
	}

	void EtoileMeshPassTextureInputSocket::perform(Texture* signal)
	{
		if(signal == NULL) return;
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		for(unsigned int i = 0; i < units.size(); ++i)
		{
			RenderUnit* unit = units[i];
			MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
			if(munit != NULL)
			{
				Mesh* m = munit->getMesh();
				for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = m->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					mt->setTexture(this->getName(), signal);
				}
			}
		}
	}

	void EtoileMeshPassTextureInputSocket::retrieve(Texture* signal)
	{
		if(signal == NULL) return;
		GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
		std::vector<RenderUnit*>& units = plugin->getRenderUnits();
		for(unsigned int i = 0; i < units.size(); ++i)
		{
			RenderUnit* unit = units[i];
			MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
			if(munit != NULL)
			{
				Mesh* m = munit->getMesh();
				for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = m->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					mt->removeTexture(this->getName());
				}
			}
		}
	}

	EtoileGLMeshRenderPassPlugin::EtoileGLMeshRenderPassPlugin(const std::string& name): EPlugin(), GLMeshRenderPass(name), _pview(NULL), _pproj(NULL), _pviewproj(NULL)
	{
		this->getType()._description = "DrawMesh";
		this->getType()._color._r = 80;
		this->getType()._color._g = 240;
		this->getType()._color._b = 250;
		this->getType()._color._a = 240;
		widget = new ParameterInputSocketsWidget();
		widget->setVisible(false);
		_pinputmesh = new EtoileMeshInputSocket();
		_pinputmeshunit = new EtoileMeshRenderUnitInputSocket;
		_pview = new EtoileMeshPassViewMatrix4fInputSocket;
		_pproj = new EtoileMeshPassProjectionMatrix4fInputSocket;
		_pviewproj = new EtoileMeshPassViewProjectionMatrix4fInputSocket;
		this->addInputSocket(_pinputmesh);
		this->addInputSocket(_pinputmeshunit);
		this->addInputSocket(_pview);
		this->addInputSocket(_pproj);
		this->addInputSocket(_pviewproj);
	}

	EtoileGLMeshRenderPassPlugin::~EtoileGLMeshRenderPassPlugin()
	{

	}


	void EtoileGLMeshRenderPassPlugin::init()
	{
		QString fileName = QFileDialog::getOpenFileName(0, QString("select MeshRenderPass"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			loadFile(fileName.toStdString());
		}else{
			QMessageBox::information(0, QString("OpenFile"),
				QString("Cannot load %1.").arg(fileName));
		}
	}

	void EtoileGLMeshRenderPassPlugin::apply()
	{
		widget->setVisible(true);
		widget->raise();
	}
	void EtoileGLMeshRenderPassPlugin::release()
	{}

	bool EtoileGLMeshRenderPassPlugin::loadFile(const std::string& filename)
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
		this->getInputSockets().clear();
		this->addInputSocket(_pinputmesh);
		this->addInputSocket(_pinputmeshunit);
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
		AABBWidget* aabbw = new AABBWidget();
		aabbw->bindPass(this);
		widget->addWidget(aabbw);
		TiXmlHandle hdl(&doc);

		TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
		while (elemRoot)
		{
			TiXmlElement *passElement = elemRoot->FirstChildElement("MeshPass");
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
						EtoileMeshPassTextureInputSocket* in = new EtoileMeshPassTextureInputSocket(name);
						this->addInputSocket(in);
					}
					else if(type.compare("float")==0)
					{
						EtoileMeshPassFloatInputSocket* in = new EtoileMeshPassFloatInputSocket(name);
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
						EtoileMeshPassIntInputSocket* in = new EtoileMeshPassIntInputSocket(name);
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

				
				passElement = passElement->NextSiblingElement("MeshPass"); // iteration
			}

			elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
		}
		
		return true;
	}

}