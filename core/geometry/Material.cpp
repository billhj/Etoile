/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Material.h
* @brief 
* @date 1/2/2011
*/

#include "Material.h"
#include <string>
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
	Material::Material(): _name(""), _ka(1), _kd(1), _ks(1), _shininess(64)
	{
		_ambient = Vec4f(0.2f,0.2f,0.2f,1);
		_diffuse = Vec4f(1,1,1,1);
		_specular = Vec4f(0.2f,0.2f,0.2f,1);
		_shininess = 64;

		_diffuseFinal = _diffuse * _kd;
		_specularFinal = _specular * _ks;
		_ambientFinal = _ambient * _ka;

		_transparency = 0;
		_dissolved_opacity = 1;

		_gpuProgram = NULL;
		/*setDiffuseTexture("emptyMap");
		setSpecularTexture("emptyMap");
		setBumpMap("emptyMap");*/	
	}

	Material::Material(const std::string& name): _name(name), _ka(1), _kd(1), _ks(1), _shininess(64)
	{
		_ambient = Vec4f(0.2f,0.2f,0.2f,1);
		_diffuse = Vec4f(1,1,1,1);
		_specular = Vec4f(0.2f,0.2f,0.2f,1);
		_shininess = 64;

		_diffuseFinal = _diffuse * _kd;
		_specularFinal = _specular * _ks;
		_ambientFinal = _ambient * _ka;

		_transparency = 0;
		_dissolved_opacity = 1;

		_gpuProgram = NULL;
		/*setDiffuseTexture("emptyMap");
		setSpecularTexture("emptyMap");
		setBumpMap("emptyMap");*/
		
	}

	Material::Material(Material& m)
	{
		_name = m._name;
		_ambient = m._ambient;
		_diffuse = m._diffuse;
		_specular = m._diffuse;
		_ka = m._ka;
		_kd = m._kd;
		_ks = m._ks;

		_transparency = m._transparency;
		_dissolved_opacity = m._dissolved_opacity;
		_ambientFinal = m._ambientFinal;
		_diffuseFinal = m._diffuseFinal;
		_specularFinal = m._specularFinal;

		_shininess = m._shininess;
		_textures = m._textures;
		_gpuProgram = m._gpuProgram;
	}

	void Material::setAmbient(const Vec4f& ambient)
	{
		_ambient = ambient;
		updateAmbient();
	}


	void Material::setDiffuse(const Vec4f& diffuse)
	{
		_diffuse = diffuse;
		updateDiffuse();
	}


	void Material::setSpecular(const Vec4f& specular)
	{
		_specular = specular;
		updateSpecular();
	}

	void Material::setAmbient(float r, float g, float b, float a)
	{
		this->_ambient[0] = r;
		this->_ambient[1] = g;
		this->_ambient[2] = b;
		this->_ambient[3] = a;
		updateAmbient();

	}
	void Material::setDiffuse(float r, float g, float b, float a)
	{
		this->_diffuse[0] = r;
		this->_diffuse[1] = g;
		this->_diffuse[2] = b;
		this->_diffuse[3] = a;
		updateDiffuse();

	}
	void Material::setSpecular(float r, float g, float b, float a)
	{
		this->_specular[0] = r;
		this->_specular[1] = g;
		this->_specular[2] = b;
		this->_specular[3] = a;
		updateSpecular();

	}


	void Material::updateDiffuse()
	{
		_diffuseFinal = _diffuse * _kd;
	}

	void Material::updateSpecular()
	{
		_specularFinal = _specular * _ks;
	}
	void Material::updateAmbient()
	{
		_ambientFinal = _ambient * _ka;
	}



	void  Material::setDiffuseTexture(Texture* t)
	{
		_textures["diffuseTexture"] =  t;
	}


	Texture* Material::getDiffuseTexture() const
	{

		std::map<std::string, Texture*>::const_iterator itor = _textures.find("diffuseTexture");
		if( itor != _textures.end())
		{
			return itor->second;
		}
		return NULL;
	}


	void  Material::setSpecularTexture(Texture* t)
	{
		_textures["specularTexture"] =  t;
	}



	Texture* Material::getSpecularTexture() const
	{
		std::map<std::string, Texture*>::const_iterator itor = _textures.find("specularTexture");
		if(itor != _textures.end())
		{
			return itor->second;
		}
		return NULL;
	}

	void Material::setBumpMap(Texture* t){
		_textures["bumpMap"] =  t;
	}

	Texture* Material::getBumpMap() const
	{
		std::map<std::string, Texture*>::const_iterator itor = _textures.find("bumpMap");
		if(itor != _textures.end())
		{
			return itor->second;
		}
		return NULL;
	}

	void Material::outputValues(std::ofstream* outfile)
	{
		(*outfile) << "Material: "<<this->_name << std::endl;
		(*outfile) << "ambient; " << this->_ambient[0]<<" " << this->_ambient[1]<<" " << this->_ambient[2]<<" " << this->_ambient[3]<<std::endl;
		(*outfile)<<"diffuse: "<<this->_diffuse[0]<<" "<<this->_diffuse[1]<<" "<< this->_diffuse[2]<<" " <<this->_diffuse[3]<<std::endl;
		(*outfile)<<"specular: "<<this->_specular[0]<<" "<<this->_specular[1]<<" "<< this->_specular[2]<<" " <<this->_specular[3]<<std::endl;
		(*outfile)<<"shininess: "<<this->_shininess<<std::endl;

	}
}
