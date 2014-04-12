/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Light.cpp
* @brief 
* @date 1/2/2011
*/

#include "Light.h"
#include <cmath>

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

	Light::Light(const std::string& name, int id) : _name(name), _id(id)
	{
		setDiffuse(0,0,0,0);
		setSpecular(0,0,0,0);
		setPosition(0,0,0,0);
		setDiffuseIntensity(1);
		setSpecularIntensity(1);
		setLookAt(0,0,0,1);
		_cutoff = 60;
	}

	Light::Light(Light& light)
	{
		 _name = light._name;
		 _diffuse = light._diffuse;
		 _specular = light._specular;
		 _diffuseIntensity = light._diffuseIntensity;
		 _specularIntensity = light._specularIntensity;
		 _diffuseFinal = light._diffuseFinal;
		 _specularFinal = light._specularFinal;
		 _position = light._position;
		 _lookAt = light._lookAt;
		 _direction = light._direction;
		 _cutoff = light._cutoff;
		_modelview = light._modelview;
		_projection = light._projection;
		_modelviewProjection = light._modelviewProjection;
		_id = light._id;
	}

	void Light::setId(int id)
	{
		_id = id;
	}

	int Light::getId()
	{
		return _id;
	}

	void Light::setLookAt(float x, float y, float z, float w)
	{
		this->_lookAt[0] = x;
		this->_lookAt[1] = y;
		this->_lookAt[2] = z;
		this->_lookAt[3] = w;
		calculateDirection();
	}

	void Light::setCutOffAngle(float angle)
	{
		_cutoff = angle;
	}

	void Light::calculateDirection()
	{
		float length = sqrt( (_lookAt[0]-_position[0]) * (_lookAt[0]-_position[0])
			+  (_lookAt[1]-_position[1]) * (_lookAt[1]-_position[1])
			+  (_lookAt[2]-_position[2]) * (_lookAt[2]-_position[2])
			);
		_direction[0] = (_lookAt[0]-_position[0]) / length;
		_direction[1] = (_lookAt[1]-_position[1]) / length;
		_direction[2] = (_lookAt[2]-_position[2]) / length;
		_direction[3] = 0;
	}



	void Light::setDiffuse(const Vec4f& diffuse)
	{
		_diffuse = diffuse;
		updateDiffuse();
	}


	void Light::setSpecular(const Vec4f& specular)
	{
		_specular = specular;
		updateSpecular();

	}

	void Light::setDiffuse(float r, float g, float b, float a)
	{
		this->_diffuse[0] = r;
		this->_diffuse[1] = g;
		this->_diffuse[2] = b;
		this->_diffuse[3] = a;
		updateDiffuse();

	}
	void Light::setSpecular(float r, float g, float b, float a)
	{
		this->_specular[0] = r;
		this->_specular[1] = g;
		this->_specular[2] = b;
		this->_specular[3] = a;
		updateSpecular();
	}

	void Light::setDiffuseIntensity(float id)
	{
		this->_diffuseIntensity = id;
		updateDiffuse();

	}
	void Light::setSpecularIntensity(float is)
	{
		this->_specularIntensity = is;
		updateSpecular();
	}

	void Light::updateDiffuse()
	{
		_diffuseFinal = _diffuse * _diffuseIntensity;
	}
	void Light::updateSpecular()
	{
		_specularFinal = _specular * _specularIntensity;
	}


	void Light::setPosition(const Vec4f& position)
	{
		_position = position;
		calculateDirection();
	}

	void Light::setPosition(float x, float y, float z, float w)
	{
		this->_position[0] = x;
		this->_position[1] = y;
		this->_position[2] = z;
		this->_position[3] = w;
		calculateDirection();
	}

	Matrix4f& Light::getModelViewMatrix()
	{
		return _modelview;
	}

	Matrix4f& Light::getProjectionMatrix()
	{
		return _projection;
	}

	Matrix4f& Light::getModelViewProjectionMatrix()
	{
		return _modelviewProjection;
	}

	void Light::updateMatrix()
	{
		_modelviewProjection = _projection * _modelview;

	}

	void Light::outputValues(std::ofstream* outfile)
	{
		(*outfile) << "light: "<< std::endl;
		(*outfile)<<"diffuse: "<<this->_diffuse.r()<<" "<<this->_diffuse.g()<<" "<< this->_diffuse.b()<<" " <<this->_diffuse.a()<<std::endl;
		(*outfile)<<"specular: "<<this->_specular.r()<<" "<<this->_specular.g()<<" "<< this->_specular.b()<<" " <<this->_specular.a()<<std::endl;
		(*outfile) << "position; " << this->getPosition().x()<<" " << this->getPosition().y()<<" " << this->getPosition().z()<<" " << this->getPosition().w()<<std::endl;

	}

}
