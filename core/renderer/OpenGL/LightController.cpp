/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file LightController.cpp
* @brief 
* @date 1/2/2011
*/

#include "LightController.h"

namespace Etoile
{

	LightController::LightController(bool activeShadowMap) : _pLight(NULL), _shadowMapActived(activeShadowMap)
	{
	}

	void LightController::setActiveShadowMap(bool activeShadowMap)
	{
		_shadowMapActived = activeShadowMap;
	}

	void LightController::bindLight(Light* light)
	{
		_pLight = light;
		_gl_Light = GL_LIGHT0 + light->getId();
		glEnable(GL_LIGHTING);
		glEnable(_gl_Light);
	}

	void LightController::unbindLight()
	{
		_pLight = NULL;
		float c[4] ={0};
		glLightfv(_gl_Light,GL_SPECULAR,c);
		glLightfv(_gl_Light,GL_DIFFUSE,c);
		glLightfv(_gl_Light,GL_POSITION, c);
	}


	void LightController::applyGLLightPosition() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		glLightfv(_gl_Light, GL_POSITION, &(_pLight->getPosition()[0]));
		printOpenGLError();
	}

	void LightController::applyGLDiffuseLight() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		printOpenGLError();
		glLightfv(_gl_Light, GL_DIFFUSE, & _pLight->getDiffuseFinal()[0]);
		printOpenGLError();
	}

	void LightController::applyGLSpecularLight() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		printOpenGLError();
		glLightfv(_gl_Light, GL_SPECULAR, &_pLight->getSpecularFinal()[0]);
		printOpenGLError();
	}

	void LightController::applyGLLightCutOffAngle() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		printOpenGLError();
		glLightf(_gl_Light, GL_SPOT_CUTOFF, _pLight->getCutOffAngle());
		printOpenGLError();
	}

	void LightController::applyGLLightDirection() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		printOpenGLError();
		glLightfv(_gl_Light, GL_SPOT_DIRECTION, &_pLight->getDirectionVector()[0]);
		printOpenGLError();
	}

	void LightController::applyGLGeneralLight() const
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}
		applyGLDiffuseLight();
		applyGLSpecularLight();
		applyGLLightPosition();
		applyGLLightCutOffAngle();
		applyGLLightDirection();
		float c[4] ={0.2f,0.2f,0.2f,0.8f};
		glLightfv(_gl_Light, GL_AMBIENT, &c[0]);
	}

	void  LightController::use() const
	{
		applyGLGeneralLight();
	}

	void LightController::updateMatrix()
	{
		if(NULL == _pLight)
		{
			std::cout<<"no light setup"<<std::endl;
			return;
		}

		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();

		glLoadIdentity();
		gluLookAt(_pLight->getPosition()[0], _pLight->getPosition()[1], _pLight->getPosition()[2], _pLight->getLookAt()[0], _pLight->getLookAt()[1], _pLight->getLookAt()[2], 0.0, 1.0, 0.0);

		glGetFloatv(GL_MODELVIEW_MATRIX , &_pLight->getModelViewMatrix()[0][0]);

		glPopMatrix();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity ();
		gluPerspective(60, 1, 0.1, 50);
		glGetFloatv(GL_PROJECTION_MATRIX , &_pLight->getProjectionMatrix()[0][0]);

		glPopMatrix();
		printOpenGLError();

		_pLight->updateMatrix();
	}


}
