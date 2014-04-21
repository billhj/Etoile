/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Material.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <fstream>
#include "math/Vectors.h"
#include "Texture.h"
#include "GpuProgram.h"
#include <map>

#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{
	class Material
	{
	public:
		Material();
		Material(const std::string& name);
		Material(Material&);

		void setAmbient(const Vec4f& ambient);
		void setDiffuse(const Vec4f& diffuse);
		void setSpecular(const Vec4f& specular);
		void setAmbient(float r, float g, float b, float a);
		void setDiffuse(float r, float g, float b, float a);
		void setSpecular(float r, float g, float b, float a);
		void setSpecularShininess(int shininess){ this->_shininess = shininess; }

		void setTransparency(float transparencyValue){ _transparency = transparencyValue;}
		float getTransparency(){ return _transparency;}
		void setDissolvedOpacity(float dissolved){ _dissolved_opacity = dissolved;}
		float getDissolvedOpacity(){return _dissolved_opacity;}

		void updateDiffuse();
		void updateSpecular();
		void updateAmbient();


		const Vec4f& getAmbient()const{ return _ambient;}
		const Vec4f& getDiffuse()const{ return _diffuse;}
		const Vec4f& getSpecular()const{ return _specular;}

		const Vec4f& getAmbientFinal()const{ return _ambientFinal;}
		const Vec4f& getDiffuseFinal()const{ return _diffuseFinal;}
		const Vec4f& getSpecularFinal()const{ return _specularFinal;}

		int getSpecularShininess()const{return _shininess;}


		void setAmbientConstant(float ka){this->_ka=ka; updateAmbient(); }
		void setDiffuseConstant(float kd){this->_kd=kd; updateDiffuse(); }
		void setSpecularConstant(float ks){this->_ks=ks; updateSpecular(); }
		float getAmbientConstant() const { return _ka;}
		float getDiffuseConstant() const { return _kd;}
		float getSpecularConstant() const { return _ks;}

		void setDiffuseTexture(Texture*);
		Texture* getDiffuseTexture() const;


		void setSpecularTexture(Texture*);
		Texture* getSpecularTexture() const;

		void setBumpMap(Texture*);
		Texture* getBumpMap() const;
		
		void setTexture(const std::string& bindingName, Texture* t)
		{
			_textures[bindingName] = t;
		}

		void removeTexture(Texture* t)
		{
			std::map<std::string, Texture*>::iterator itor = _textures.begin();
			for(; itor != _textures.end(); ++itor)
			{
				if(itor->second == t)
				{
					_textures.erase(itor);
				}
			}
		}

		void removeTexture(const std::string& name)
		{
			std::map<std::string, Texture*>::iterator itor = _textures.find(name);
			if(itor != _textures.end())
			{
				_textures.erase(itor);
			}
		}

		std::map<std::string, Texture*>& getTextures(){ return _textures;}

		void setGpuProgram(GpuProgram* gpuProgram)
		{
			_gpuProgram = gpuProgram;
		}

		GpuProgram* getGpuProgram() const
		{
			return _gpuProgram;
		}

		void outputValues(std::ofstream* outfile);
		const std::string getName(){return _name;}
		void setName(const std::string& name){ _name = name;}

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _materialname;
			ar & _ambient;
			ar & _diffuse;
			ar & _specular;
			ar & _ka;
			ar & _kd;
			ar & _ks;

			ar & _transparency;
			ar & _dissolved_opacity;
			ar & _ambientFinal;
			ar & _diffuseFinal;
			ar & _specularFinal;

			ar & _shininess;
			ar & _textures;
			ar & _gpuProgram;
			ar & _materialname;
		}
#endif

	private:

		Vec4f _ambient;
		Vec4f _diffuse;
		Vec4f _specular;
		float _ka;
		float _kd;
		float _ks;

		float _transparency;
		float _dissolved_opacity;
		Vec4f _ambientFinal;
		Vec4f _diffuseFinal;
		Vec4f _specularFinal;

		int _shininess;
		std::map<std::string, Texture*> _textures;
		GpuProgram* _gpuProgram;
		std::string _name;
	};

	//typedef std::map<std::string, Material*> MaterialList;
}
