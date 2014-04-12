/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RendererInit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/GLMeshRenderPass.h"
#include "renderer/OpenGL/QuadRenderPass.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/RenderTarget.h"
#include "tinyxml/tinyxml.h"
#include "util/File.h"

namespace Etoile
{
	class RendererInit
	{
		int _width;
		int _height;
		std::string _file;
	public:
		RendererInit():_width(800), _height(600){}
		bool reInit()
		{
			return init(_file);
		}

		bool init(const std::string& filepath)
		{
			TiXmlDocument doc(filepath.c_str());
			if(!doc.LoadFile()){
				std::cout << "erreur while loading" << " renderer " <<filepath<< std::endl;
				std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
				return false;
			}
			_file = filepath;
			TiXmlHandle hdl(&doc);
			TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
			std::string directory = File::getFilePath(filepath); 
			while (elemRoot)
			{
				TiXmlElement *elemRenderer = elemRoot->FirstChildElement("renderer");
				while(elemRenderer)
				{
					const char* name = elemRenderer->Attribute("name");
					renderer->setName(name);

					TiXmlElement *elemPass = elemRenderer->FirstChildElement("pass");
					while(elemPass)
					{
						RenderPass* pass;

						const char* name = elemPass->Attribute("name");
						if(name != NULL && !std::string(name).empty())
						{
							const char* passtype = elemPass->Attribute("passtype");
							if(passtype != NULL && !std::string(passtype).empty())
							{
								if(std::string(passtype).compare("mesh") == 0)
								{
									pass = new GLMeshRenderPass(name);
									std::vector<Resource*> meshes = renderer->getMeshManager()->getAll();
									for(unsigned int i = 0 ; i < meshes.size(); ++i)
									{
										Mesh* _pMesh = (Mesh*)meshes[i];
										((GLMeshRenderPass*)pass)->addMesh(_pMesh);
									}
								}
								else if(std::string(passtype).compare("quad") == 0)
								{
									pass = new QuadRenderPass(name);
									const char* gpu = elemPass->Attribute("gpu");
									if(gpu != NULL && !std::string(gpu).empty())
									{
										Material* mt= new Material(name); 
										mt->setGpuProgram(gpu);
										((QuadRenderPass*)pass)->setMaterial(name);
										renderer->getMaterialManager()->addMaterial(mt);
										addInput(elemPass, mt, renderer);
									}
									else
									{
										std::cerr<< "RendererInit: pass gpu error!"<<std::endl;
									}
								}
								else
								{
									std::cerr<< "RendererInit: passtype error0!"<<std::endl;
								}

								if(pass != NULL)
								{
									int x, y; 
									if(TIXML_NO_ATTRIBUTE != elemPass->QueryIntAttribute("w", &x))
									{
										_width = x;
									}
									if(TIXML_NO_ATTRIBUTE != elemPass->QueryIntAttribute("h", &y))
									{
										_height = y;
									}

									addOutput(elemPass, pass, renderer);
									renderer->addRenderPass(pass);
								}

							}else
							{
								std::cerr<< "RendererInit: passtype error!"<<std::endl;
							}
						}else
						{
							std::cerr<< "RendererInit: pass name error!"<<std::endl;
						}


						elemPass = elemPass->NextSiblingElement("pass");
					}
					elemRenderer = elemRenderer->NextSiblingElement("renderer");
				}
				elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
			}
			return true;
		}

		void addInput(TiXmlElement* elemPass, Material* mt, Renderer* renderer)
		{
			TiXmlElement *elemInput = elemPass->FirstChildElement("input");
			if(elemInput == NULL) return;
			TiXmlElement *elemTexture2d = elemInput->FirstChildElement("texture2d");
			while(elemTexture2d)
			{
				const char* name = elemTexture2d->Attribute("name");
				const char* bindingname = elemTexture2d->Attribute("bindingname");
				if(name != NULL)
				{
					if(bindingname == NULL || std::string(bindingname).empty())
					{
						mt->setTexture(name, name);
					}else
					{
						mt->setTexture(bindingname, name);
					}
				}
				elemTexture2d = elemTexture2d->NextSiblingElement("texture2d"); // iteration
			}
		}

		void addInput(TiXmlElement* elemPass, Renderer* renderer)
		{
			TiXmlElement *elemInput = elemPass->FirstChildElement("input");
			if(elemInput == NULL) return;
			TiXmlElement *elemTexture2d = elemInput->FirstChildElement("texture2d");
			while(elemTexture2d)
			{
				const char* name = elemTexture2d->Attribute("name");
				const char* bindingname = elemTexture2d->Attribute("bindingname");

				for(unsigned int i = 0 ; i < renderer->getMeshManager()->getAll().size(); ++i)
				{
					Mesh* mesh = (Mesh*) renderer->getMeshManager()->getAll()[i];
					std::vector<SubMesh*> subs = mesh->getSubMeshList();
					for(unsigned int j = 0; j < subs.size(); ++j)
					{
						SubMesh* sub = subs[j];
						Material* mt = renderer->getMaterialManager()->getMaterialByName(sub->getMaterial());
						if(name != NULL)
						{
							if(bindingname == NULL || std::string(bindingname).empty())
							{
								mt->setTexture(name, name);
							}else
							{
								mt->setTexture(bindingname, name);
							}
						}
					}
				}
				elemTexture2d = elemTexture2d->NextSiblingElement("texture2d"); // iteration
			}
		}

		void addOutput(TiXmlElement* elemPass, RenderPass* pass)
		{
			TiXmlElement *elemOutput = elemPass->FirstChildElement("output");
			if(elemOutput == NULL) return;
			TiXmlElement *elemTexture2d = elemOutput->FirstChildElement("texture2d");
			TextureRenderTarget* target = new TextureRenderTarget();
			target->getType()._name = pass->getName();
			while(elemTexture2d)
			{
				const char* name = elemTexture2d->Attribute("name");
				Texture* t = renderer->getTextureManager()->getTextureByName(name);
				if(t != NULL && t->getWidth() == _width && t->getHeight() == _height)
				{

				}
				else
				{
					if(t == NULL) delete t;
					int mipmap = 0;
					if(TIXML_NO_ATTRIBUTE == elemTexture2d->QueryIntAttribute("mipmap", &mipmap) || mipmap==0)
					{
						mipmap = 0;
					}
					const char* internalFormat = elemTexture2d->Attribute("internalFormat");
					if(internalFormat == NULL)
					{
						internalFormat = "GL_RGBA16F_ARB";
					}
					const char* pixelFormat = elemTexture2d->Attribute("pixelformat");
					if(pixelFormat == NULL)
					{
						pixelFormat = "GL_RGBA";
					}
					const char* pixeltype = elemTexture2d->Attribute("pixeltype");
					if(pixeltype==NULL)
					{
						pixeltype = "GL_FLOAT";
					}

					GLTexture2D* tx = new GLTexture2D(name);
					std::cout<<"creating texture  "<< name <<" "<< _width<<" "<<_height<<std::endl;
					tx->create(_width, _height, 1, GetTextureInternalFormat(internalFormat), GetTexturePixelDataFormat(pixelFormat), GetTexturePixelDataType(pixeltype), 0, mipmap > 0 ? true : false);
					renderer->getTextureManager()->addTexture(tx);
				}
				target->addTextureTarget(name);
				elemTexture2d = elemTexture2d->NextSiblingElement("texture2d"); // iteration
			}
			if(target->getTextureTargets().size() > 0)
			{
				pass->addRenderTarget(target);
			}
		}

	};

}
