/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GpuProgramInit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/GLSLGpuProgram.h"
#include "renderer/OpenGL/VertexShader.h"
#include "renderer/OpenGL/FragmentShader.h"
#include "renderer/OpenGL/TessCtrlShader.h"
#include "renderer/OpenGL/TessEvalShader.h"
#include "renderer/OpenGL/GeometryShader.h"
#include "renderer/OpenGL/ComputeShader.h"
#include "tinyxml/tinyxml.h"
#include "util/File.h"

namespace Etoile
{
	class GpuProgramInit
	{
		std::string _file;
		std::vector<GLSLGpuProgram*> _gpupro;
	public:
		GpuProgramInit(){}
		bool reInit()
		{
			return init(_file);
		}
		std::vector<GLSLGpuProgram*> getGpuPrograms(){return _gpupro;}
		bool init(const std::string& filepath)
		{
			TiXmlDocument doc(filepath.c_str());
			if(!doc.LoadFile()){
				std::cout << "erreur while loading" << " gpugrams " <<filepath<< std::endl;
				std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
				return false;
			}
			_file = filepath;
			TiXmlHandle hdl(&doc);
			TiXmlElement *elemRoot = hdl.FirstChildElement("Etoile").Element();
			std::string directory = File::getFilePath(filepath); 
			while (elemRoot)
			{
				TiXmlElement *elemGpuProgram = elemRoot->FirstChildElement("gpuprogram");
				while(elemGpuProgram)
				{
					const char* gpuName = elemGpuProgram->Attribute("name");
					if(gpuName)
					{
						GLSLGpuProgram* gpuprogram = new GLSLGpuProgram(gpuName);
						ShaderProgram* shaderProgram = new ShaderProgram();
						const char* vertex = elemGpuProgram->Attribute("vertex");
						if(vertex != NULL)
						{
							VertexShader* vs = new VertexShader(directory + vertex);
							shaderProgram->addShader(vs);
						}
						const char* fragment = elemGpuProgram->Attribute("fragment");
						if(fragment != NULL)
						{
							FragmentShader* fs = new FragmentShader(directory + fragment);
							shaderProgram->addShader(fs);
						}
						const char* control = elemGpuProgram->Attribute("control");
						if(control != NULL)
						{
							TessCtrlShader* cs = new TessCtrlShader(directory + control);
							shaderProgram->addShader(cs);
						}
						const char* evaluation = elemGpuProgram->Attribute("evaluation");
						if(evaluation != NULL)
						{
							TessEvalShader* es = new TessEvalShader(directory + evaluation);
							shaderProgram->addShader(es);
						}
						const char* geometry = elemGpuProgram->Attribute("geometry");
						if(geometry != NULL)
						{
							GeometryShader* gs = new GeometryShader(directory + geometry);
							shaderProgram->addShader(gs);
						}
						const char* compute = elemGpuProgram->Attribute("compute");
						if(compute != NULL)
						{
							ComputeShader* cs = new ComputeShader(directory + compute);
							shaderProgram->addShader(cs);
						}
						shaderProgram->link();
						gpuprogram->loadShader(shaderProgram);
						_gpupro.push_back(gpuprogram);
					}
					else
					{
						std::cout<<"readingGpuProgram <" << gpuName<< "> has reading problem!"<<std::endl;
					}
					 elemGpuProgram = elemGpuProgram->NextSiblingElement("gpuprogram");
				}
				elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
			}
			return true;
		}
	};

}
