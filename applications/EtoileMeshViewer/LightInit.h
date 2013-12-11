/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file LightInit.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/Light.h"
#include "tinyxml/tinyxml.h"
#include "util/File.h"

namespace Etoile
{
	class LightInit
	{
		std::string _file;
		std::vector<Light*> _lights;
	public:
		LightInit(){}
		bool reInit()
		{
			return init(_file);
		}
		std::vector<Light*> getLights()
		{
			return _lights;
		}

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
				int i = 0;
				TiXmlElement *elemLight = elemRoot->FirstChildElement("light");
				while(elemLight)
				{
					const char* name = elemLight->Attribute("name");
					if(name != NULL && !std::string(name).empty())
					{
						Light* light = new Light(name, i);
						_lights.push_back(light);
						TiXmlElement *elemDiffuse = elemLight->FirstChildElement("diffuse");
						while (elemDiffuse)
						{
							float r = 1;
							if(TIXML_NO_ATTRIBUTE == elemDiffuse->QueryFloatAttribute("r", &r))
							{
								std::cout<<"light error loading : diffuse" <<std::endl;
							}

							float g = 1;
							if(TIXML_NO_ATTRIBUTE == elemDiffuse->QueryFloatAttribute("g", &g))
							{
								std::cout<<"light error loading : diffuse" <<std::endl;
							}

							float b = 1;
							if(TIXML_NO_ATTRIBUTE == elemDiffuse->QueryFloatAttribute("b", &b))
							{
								std::cout<<"light error loading : diffuse" <<std::endl;
							}

							float a = 1;
							if(TIXML_NO_ATTRIBUTE == elemDiffuse->QueryFloatAttribute("a", &a))
							{
								std::cout<<"light error loading : diffuse" <<std::endl;
							}

							float intensity = 1;
							if(TIXML_NO_ATTRIBUTE == elemDiffuse->QueryFloatAttribute("intensity", &intensity))
							{
								std::cout<<"light error loading : diffuse" <<std::endl;
							}

							light->setDiffuse(r,g,b,a);
							light->setDiffuseIntensity(intensity);

							elemDiffuse = elemDiffuse->NextSiblingElement("diffuse"); // iteration
						}


						TiXmlElement *elemSpecular = elemLight->FirstChildElement("specular");
						while (elemSpecular)
						{
							float r = 1;
							if(TIXML_NO_ATTRIBUTE == elemSpecular->QueryFloatAttribute("r", &r))
							{
								std::cout<<"light error loading : specular" <<std::endl;
							}

							float g = 1;
							if(TIXML_NO_ATTRIBUTE == elemSpecular->QueryFloatAttribute("g", &g))
							{
								std::cout<<"light error loading : specular" <<std::endl;
							}

							float b = 1;
							if(TIXML_NO_ATTRIBUTE == elemSpecular->QueryFloatAttribute("b", &b))
							{
								std::cout<<"light error loading : specular" <<std::endl;
							}

							float a = 1;
							if(TIXML_NO_ATTRIBUTE == elemSpecular->QueryFloatAttribute("a", &a))
							{
								std::cout<<"light error loading : specular" <<std::endl;
							}

							float intensity = 1;
							if(TIXML_NO_ATTRIBUTE == elemSpecular->QueryFloatAttribute("intensity", &intensity))
							{
								std::cout<<"light error loading : specular" <<std::endl;
							}

							light->setSpecular(r,g,b,a);
							light->setSpecularIntensity(intensity);
							elemSpecular = elemSpecular->NextSiblingElement("specular"); // iteration
						}

						TiXmlElement *elemPosition = elemLight->FirstChildElement("position");
						while (elemPosition)
						{
							float x = 0;
							if(TIXML_NO_ATTRIBUTE == elemPosition->QueryFloatAttribute("x", &x))
							{
								std::cout<<"light error loading : position" <<std::endl;
							}

							float y = 0;
							if(TIXML_NO_ATTRIBUTE == elemPosition->QueryFloatAttribute("y", &y))
							{
								std::cout<<"light error loading : position" <<std::endl;
							}

							float z = 0;
							if(TIXML_NO_ATTRIBUTE == elemPosition->QueryFloatAttribute("z", &z))
							{
								std::cout<<"light error loading : position" <<std::endl;
							}
							light->setPosition(x, y , z, 1);
							elemPosition = elemPosition->NextSiblingElement("position"); // iteration
						}
						TiXmlElement *elemDirection = elemLight->FirstChildElement("lookat");
						while (elemDirection)
						{
							float x = 0;
							if(TIXML_NO_ATTRIBUTE == elemDirection->QueryFloatAttribute("x", &x))
							{
								std::cout<<"light error loading : lookat" <<std::endl;
							}

							float y = 0;
							if(TIXML_NO_ATTRIBUTE == elemDirection->QueryFloatAttribute("y", &y))
							{
								std::cout<<"light error loading : lookat" <<std::endl;
							}

							float z = 0;
							if(TIXML_NO_ATTRIBUTE == elemDirection->QueryFloatAttribute("z", &z))
							{
								std::cout<<"light error loading : lookat" <<std::endl;
							}

							float angle = 60;
							if(TIXML_NO_ATTRIBUTE == elemDirection->QueryFloatAttribute("angle", &angle))
							{
								std::cout<<"light error loading : lookat" <<std::endl;
							}

							light->setLookAt(x, y, z, 1);
							light->setCutOffAngle(angle);

							elemDirection = elemDirection->NextSiblingElement("lookat"); // iteration
						}
					}
					++i;
					elemLight = elemLight->NextSiblingElement("light");
				}
				elemRoot = elemRoot->NextSiblingElement("Etoile"); // iteration
			}
			return true;
		}
	};

}
