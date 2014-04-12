/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSceneRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLRenderPass.h"
#include "GLSceneRenderUnit.h"
#include "geometry/SceneEntity.h"
#include "geometry/Primitive.h"

namespace Etoile
{
	/*enum RenderMode
	{
	RenderMode_Immediate = 0,				///< 			
	RenderMode_GPUVBO,   
	RenderMode_AvancedGPUVBO,         ///< Invalid type
	};*/

	class GLSceneRenderPass : public GLRenderPass
	{
		GpuProgram * _pGpuCommon;
		GLSceneRenderUnit* _pUnit;
	public:
		GLSceneRenderPass(const std::string& name): GLRenderPass(name), _pGpuCommon(NULL)
		{
			_pUnit = new GLSceneRenderUnit("DrawScene");
			_renderUnits.push_back(_pUnit);
		}

		void setCommonGpuProgram(GpuProgram * gpu)
		{
			_pGpuCommon = gpu;
		}

		virtual void addRenderUnit(RenderUnit* ru) override
		{
			/*GLSceneRenderUnit* unit = dynamic_cast<GLSceneRenderUnit*>(ru);
			if(unit != NULL)
			{
				if(unit->getScene()!= NULL)
				{
					_pUnit = unit;
				}
				Scene* scene = unit->getScene();
				for(unsigned int  j = 0; j < scene->getSceneNodes().size(); ++j)
				{
					SceneNode* pnode = scene->getSceneNodes()[j];
					for(unsigned int i = 0; i < pnode->getMovableObjects().size(); ++i)
					{
						MovableObject* obj = pnode->getMovableObjects()[i];
						SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
						if(entity != NULL)
						{
							Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
							if(mesh != NULL)
							{
								for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
								{
									SubMesh* submesh = mesh->getSubMeshList()[j];
									Material* mt = submesh->getMaterial();
									if(_pGpuCommon != NULL)
									{
										mt->setGpuProgram(_pGpuCommon);
									}
								}
							}
						}
					}
				}
			}*/
		}

		virtual void addScene(Scene* scene)
		{
			if(scene != NULL)
			{
				_pUnit->setScene(scene);
				for(unsigned int  j = 0; j < scene->getSceneNodes().size(); ++j)
				{
					SceneNode* pnode = scene->getSceneNodes()[j];
					for(unsigned int i = 0; i < pnode->getMovableObjects().size(); ++i)
					{
						MovableObject* obj = pnode->getMovableObjects()[i];
						SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
						Primitive* prim = dynamic_cast<Primitive*>(obj);
						if(entity != NULL)
						{
							Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
							if(mesh != NULL)
							{
								for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
								{
									SubMesh* submesh = mesh->getSubMeshList()[j];
									Material* mt = submesh->getMaterial();
									if(_pGpuCommon != NULL)
									{
										mt->setGpuProgram(_pGpuCommon);
									}
								}
							}
							//Skeleton* sk = dynamic_cast<Skeleton*>(entity->getMesh());
						}
						if(prim != NULL)
						{
							Mesh* mesh = dynamic_cast<Mesh*>(prim->getMesh());
							if(mesh != NULL)
							{
								for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
								{
									SubMesh* submesh = mesh->getSubMeshList()[j];
									Material* mt = submesh->getMaterial();
									if(_pGpuCommon != NULL)
									{
										mt->setGpuProgram(_pGpuCommon);
									}
								}
							}
						}
					}
				}
			}
		}

		void removeScene(Scene* scene)
		{
			if(scene == _pUnit->getScene())
			{
				_pUnit->setScene(NULL);
			}
		}

		virtual void draw() override
		{
			useRenderTargets();
			//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			for(unsigned int i = 0; i < _renderUnits.size(); ++i)
			{
				_renderUnits[i]->draw();
			}
			unUseRenderTargets();
		}

	};
}
