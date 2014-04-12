/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLQuadRenderPassPlugin.cpp
* @brief 
* @date 1/2/2011
*/

#include "EtoileGLQuadRenderPassPlugin.h"
#include "util/File.h"
#include "QtTextureLoader.h"
#include "tinyxml/tinyxml.h"
#include <QFileDialog>
#include <QMessageBox>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


Etoile::EPlugin* loadEtoileGLQuadRenderPassPlugin()
{
	return new Etoile::EtoileGLQuadRenderPassPlugin("CommonQuadRenderPass");
}

namespace Etoile
{

	EtoileQuadPassTextureInputSocket::EtoileQuadPassTextureInputSocket(const std::string& name):TextureInputSocket(name)
	{
	}

	void EtoileQuadPassTextureInputSocket::perform(Texture* signal)
	{
		if(signal == NULL) return;
		GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
		Material* mt = plugin->getMaterial();
		mt->setTexture(this->getName(), signal);
	}

	void EtoileQuadPassTextureInputSocket::retrieve(Texture* signal)
	{
		if(signal == NULL) return;
		GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
		Material* mt = plugin->getMaterial();
		mt->removeTexture(this->getName());	
	}

	EtoileGLQuadRenderPassPlugin::EtoileGLQuadRenderPassPlugin(const std::string& name): EPlugin(), GLQuadRenderPass(name), _pview(NULL), _pproj(NULL), _pviewproj(NULL)
	{
		this->getType()._description = "DrawQuad";
		this->getType()._color._r = 80;
		this->getType()._color._g = 240;
		this->getType()._color._b = 230;
		this->getType()._color._a = 240;
		widget = new ParameterInputSocketsWidget();
		widget->setVisible(false);

		initMatrixSocket();
		/*QString fileName = QFileDialog::getOpenFileName(0, QString("select QuadRenderPass"), QDir::currentPath());
		if (!fileName.isEmpty()) {
			loadFile(fileName.toStdString());
		}else{
			QMessageBox::information(0, QString("OpenFile"),
				QString("Cannot load %1.").arg(fileName));
		}*/
	}

	EtoileGLQuadRenderPassPlugin::~EtoileGLQuadRenderPassPlugin()
	{

	}

	void EtoileGLQuadRenderPassPlugin::initMatrixSocket()
	{

		_pview = new EtoileQuadPassViewMatrix4fInputSocket;
		_pproj = new EtoileQuadPassProjectionMatrix4fInputSocket;
		_pviewproj = new EtoileQuadPassViewProjectionMatrix4fInputSocket;
		this->addInputSocket(_pview);
		this->addInputSocket(_pproj);
		this->addInputSocket(_pviewproj);


	}

	void EtoileGLQuadRenderPassPlugin::init()
	{
		
	}

	void EtoileGLQuadRenderPassPlugin::apply()
	{
		widget->setVisible(true);
		widget->raise();
	}
	void EtoileGLQuadRenderPassPlugin::release()
	{}

	bool EtoileGLQuadRenderPassPlugin::loadFile(const std::string& filename)
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
		TiXmlHandle hdl(&doc);

		TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
		while (elemRoot)
		{
			TiXmlElement *passElement = elemRoot->FirstChildElement("QuadPass");
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
						files[SHADERTYPE_VERTEX] = path + file;
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
					this->getMaterial()->setGpuProgram(glsl);
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
						EtoileQuadPassTextureInputSocket* in = new EtoileQuadPassTextureInputSocket(name);
						this->addInputSocket(in);
					}
					else if(type.compare("float")==0)
					{
						EtoileQuadPassFloatInputSocket* in = new EtoileQuadPassFloatInputSocket(name);
						in->setNode(this);
						//this->addInputSocket(in);
						FloatInputSocketWidget* socketwidget = new FloatInputSocketWidget(widget);
						float value = 0;
						if(TIXML_NO_ATTRIBUTE!=inputsElement->QueryFloatAttribute("value", &value))
						{
							in->perform(&value);
						}
						socketwidget->bindParameter(in, value);
						widget->addWidget(socketwidget);
					}
					else if(type.compare("int")==0)
					{
						EtoileQuadPassIntInputSocket* in = new EtoileQuadPassIntInputSocket(name);
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


				passElement = passElement->NextSiblingElement("QuadPass"); // iteration
			}

			elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
		}
		return true;
	}

}