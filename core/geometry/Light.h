/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Light.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <fstream>
#include "math/Matrix.h"
#include "math/Vectors.h"

namespace Etoile
{

	class Light
	{
	public:
		Light(const std::string& name = "", int id = 0);
		Light(Light& light);

		void setId(int id);
		int getId();
		void setDiffuse(const Vec4f& diffuse);
		void setSpecular(const Vec4f& specular);
		void setDiffuse(float r = 1, float g = 1, float b = 1, float a = 1);
		void setSpecular(float r = 1, float g = 1, float b = 1, float a = 1);
		void updateDiffuse();
		void updateSpecular();

		const Vec4f& getDiffuseFinal() const{ return _diffuseFinal;}
		const Vec4f& getSpecularFinal() const{ return _specularFinal;}

		const Vec4f& getDiffuse() const{ return _diffuse;}
		const Vec4f& getSpecular() const{ return _specular;}

		void setDiffuseIntensity(float id = 1);
		void setSpecularIntensity(float is = 1);
		float getDiffuseIntensity()const{return _diffuseIntensity;}
		float getSpecularIntensity()const{return _specularIntensity;}

		void setPosition(const Vec4f& position);
		void setPosition(float x = 1, float y = 1, float z = 1, float w = 0);
		const Vec4f& getPosition() const{ return _position;}

		void setLookAt(float x = 0, float y = 0, float z = 0, float w = 0);
		const Vec4f& getLookAt() const{return _lookAt;}
		const Vec4f& getDirectionVector() const{ return _direction;}

		void setCutOffAngle(float angle);
		float getCutOffAngle()const{return _cutoff;}

		Matrix4f& getModelViewMatrix();
		Matrix4f& getProjectionMatrix();
		Matrix4f& getModelViewProjectionMatrix();

		void updateMatrix();

		void outputValues(std::ofstream* outfile);
	private:
		void calculateDirection();


	private:

		Vec4f _diffuse;
		Vec4f _specular;
		float _diffuseIntensity;
		float _specularIntensity;

		Vec4f _diffuseFinal;
		Vec4f _specularFinal;

		Vec4f _position;
		Vec4f _lookAt;
		Vec4f _direction;
		float _cutoff;

		Matrix4f _modelview, _projection, _modelviewProjection;
		int _id;
		std::string _name;
	};

}
