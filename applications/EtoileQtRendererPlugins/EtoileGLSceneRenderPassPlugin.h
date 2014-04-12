/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLSceneRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/TextureSocket.h"
#include "renderer/OpenGL/GLSceneRenderPass.h"
#include "renderer/OpenGL/GLSLGpuProgram.h"
#include "ParameterInputSocketsWidget.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/SceneNodeSocket.h"
#include "SceneEditor.h"

Etoile::EPlugin* loadEtoileGLSceneRenderPassPlugin();

namespace Etoile
{
	class EtoileSceneInputSocket : public SceneInputSocket
	{
	public:
		EtoileSceneInputSocket(const std::string name = "Scene");
		virtual void perform(Scene* signal) override;
		virtual void retrieve(Scene* signal) override;
	};

	class EtoileScenePassTextureInputSocket : public TextureInputSocket
	{
	public:
		EtoileScenePassTextureInputSocket(const std::string& name);
		virtual void perform(Texture* signal) override;
		virtual void retrieve(Texture* signal) override;
	};

	class EtoileScenePassIntInputSocket : public IntInputSocket
	{
	public:
		EtoileScenePassIntInputSocket(const std::string& name):IntInputSocket(name){}
		virtual void perform(int* signal) override
		{
			if(signal == NULL) return;
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(munit != NULL)
				{
					Scene* scene = munit->getScene();
					if(scene == NULL) continue;
					for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* node = scene->getSceneNodes()[j];
						for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
						{
							MovableObject* obj = node->getMovableObjects()[k];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
							Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(obj);
							if(plane != NULL)
							{
								Etoile::Mesh* mesh = dynamic_cast<Etoile::Mesh*>(plane->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										Etoile::SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
						}
					}
				}
			}

		}
		virtual void retrieve(int* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileScenePassFloatInputSocket : public FloatInputSocket
	{
	public:
		EtoileScenePassFloatInputSocket(const std::string& name) : FloatInputSocket(name)
		{
		}

		virtual void perform(float* signal) override
		{
			if(signal == NULL) return;
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(munit != NULL)
				{
					Scene* scene = munit->getScene();
					if(scene == NULL) continue;
					for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* node = scene->getSceneNodes()[j];
						for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
						{
							MovableObject* obj = node->getMovableObjects()[k];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
							Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(obj);
							if(plane != NULL)
							{
								Etoile::Mesh* mesh = dynamic_cast<Etoile::Mesh*>(plane->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										Etoile::SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
						}
					}
				}
			}

		}
		virtual void retrieve(float* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileScenePassViewMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileScenePassViewMatrix4fInputSocket(const std::string& name = "In_ViewMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(munit != NULL)
				{
					Scene* scene = munit->getScene();
					if(scene == NULL) continue;
					for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* node = scene->getSceneNodes()[j];
						for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
						{
							MovableObject* obj = node->getMovableObjects()[k];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
							Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(obj);
							if(plane != NULL)
							{
								Etoile::Mesh* mesh = dynamic_cast<Etoile::Mesh*>(plane->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										Etoile::SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
						}
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};


	class EtoileScenePassProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileScenePassProjectionMatrix4fInputSocket(const std::string& name = "In_ProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(munit != NULL)
				{
					Scene* scene = munit->getScene();
					if(scene == NULL) continue;
					for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* node = scene->getSceneNodes()[j];
						for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
						{
							MovableObject* obj = node->getMovableObjects()[k];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
							Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(obj);
							if(plane != NULL)
							{
								Etoile::Mesh* mesh = dynamic_cast<Etoile::Mesh*>(plane->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										Etoile::SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
						}
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileScenePassViewProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileScenePassViewProjectionMatrix4fInputSocket(const std::string& name = "In_ViewProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLSceneRenderPass* plugin = (GLSceneRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				GLSceneRenderUnit* munit = dynamic_cast<GLSceneRenderUnit*>(units[i]);
				if(munit != NULL)
				{
					Scene* scene = munit->getScene();
					if(scene == NULL) continue;
					for(unsigned int j = 0; j < scene->getSceneNodes().size(); ++j)
					{
						SceneNode* node = scene->getSceneNodes()[j];
						for(unsigned int k = 0; k < node->getMovableObjects().size(); ++k)
						{
							MovableObject* obj = node->getMovableObjects()[k];
							SceneEntity* entity = dynamic_cast<SceneEntity*>(obj);
							if(entity != NULL)
							{
								Mesh* mesh = dynamic_cast<Mesh*>(entity->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
							Etoile::Plane* plane = dynamic_cast<Etoile::Plane*>(obj);
							if(plane != NULL)
							{
								Etoile::Mesh* mesh = dynamic_cast<Etoile::Mesh*>(plane->getMesh());
								if(mesh != NULL)
								{
									for(unsigned int l = 0; l < mesh->getSubMeshList().size(); ++l)
									{
										Etoile::SubMesh* submesh = mesh->getSubMeshList()[l];
										Material* mt = submesh->getMaterial();
										GpuProgram* gpu = mt->getGpuProgram();
										GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
										if(ggpu != NULL)
											ggpu->setUniformVariable(this->_type._name, *signal);
									}
								}
							}
						}
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileGLSceneRenderPassPlugin : public EPlugin, public GLSceneRenderPass 
	{
	public:
		EtoileGLSceneRenderPassPlugin(const std::string& name);
		~EtoileGLSceneRenderPassPlugin();
		bool loadFile(const std::string& filename);
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	protected:
		//Scene* _scene;
		EtoileSceneInputSocket* _psceneinput;
		ParameterInputSocketsWidget* widget;
		EtoileScenePassViewMatrix4fInputSocket* _pview;
		EtoileScenePassProjectionMatrix4fInputSocket* _pproj;
		EtoileScenePassViewProjectionMatrix4fInputSocket * _pviewproj;
	};
}

