/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSceneRenderUnit.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLSceneRenderUnit.h"
#include "GLRenderSkeleton.h"
#include "geometry/SceneEntity.h"
#include "geometry/Primitive.h"
#include "GLRenderMesh.h"
#include "GLTexture2D.h"

namespace Etoile
{

	void GLSceneRenderUnit::draw()
	{
		if(_pscene == NULL) return;
		SceneSky* sky = _pscene->getSkye();
		if(sky != NULL) sky->perform();
		for(unsigned int j = 0; j < _pscene->getSceneNodes().size(); ++j)
		{
			SceneNode* node = _pscene->getSceneNodes()[j];
			//node->updateTransform();
			for(unsigned int i = 0; i < node->getMovableObjects().size(); ++i)
			{
				MovableObject* obj = node->getMovableObjects()[i];
				if(obj->isVisible())
				{
					obj->perform(node->getGLModelMatrix());
				}
				
			}
		}
	}

}
