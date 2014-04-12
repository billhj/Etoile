/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file LightController.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "geometry/Light.h"

namespace Etoile
{

	class LightController
	{

	public:
		LightController(bool activeShadowMap = true);
		void setActiveShadowMap(bool activeShadowMap = true);
		void setShadowMapName(const std::string& name){_shadowMap = name;}
		const std::string getShadowMapName(){return _shadowMap;}

		void setShadowMapGpuProgramName(const std::string& name){_shadowMapGpuProgram = name;}
		const std::string getShadowMapGpuProgramName(){return _shadowMapGpuProgram;}

		Light* getLight(){return _pLight;}
		void bindLight(Light*);
		void unbindLight();

		void applyGLDiffuseLight() const;
		void applyGLSpecularLight() const;
		void applyGLLightPosition() const;
		void applyGLLightCutOffAngle() const;
		void applyGLLightDirection() const;

		void applyGLGeneralLight() const;
		void use() const;
		void updateMatrix();

	private:
		Light* _pLight;
		GLenum _gl_Light;
		std::string _shadowMap;
		std::string _shadowMapGpuProgram;
		bool _shadowMapActived;
	};

}
