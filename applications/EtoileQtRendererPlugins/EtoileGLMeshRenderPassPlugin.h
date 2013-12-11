/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLMeshRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/TextureSocket.h"
#include "renderer/OpenGL/GLMeshRenderPass.h"
#include "renderer/OpenGL/GLSLGpuProgram.h"
#include "ParameterInputSocketsWidget.h"
#include "module/flowgraph/GeneralSocket.h"
#include "module/flowgraph/MeshSocket.h"

Etoile::EPlugin* loadEtoileGLMeshRenderPassPlugin();

namespace Etoile
{
	class EtoileMeshInputSocket : public MeshInputSocket
	{
	public:
		EtoileMeshInputSocket(const std::string name = "Mesh");
		virtual void perform(Mesh* signal) override;
		virtual void retrieve(Mesh* signal) override;
	};

	class EtoileMeshRenderUnitInputSocket : public TemplateInputSocket<MeshRenderUnit>
	{
	public:
		EtoileMeshRenderUnitInputSocket(const std::string name = "MeshRenderUnit");
		virtual void perform(MeshRenderUnit* signal) override;
		virtual void retrieve(MeshRenderUnit* signal) override;
	};

	class EtoileMeshPassTextureInputSocket : public TextureInputSocket
	{
	public:
		EtoileMeshPassTextureInputSocket(const std::string& name);
		virtual void perform(Texture* signal) override;
		virtual void retrieve(Texture* signal) override;
	};

	class EtoileMeshPassIntInputSocket : public IntInputSocket
	{
	public:
		EtoileMeshPassIntInputSocket(const std::string& name):IntInputSocket(name){}
		virtual void perform(int* signal) override
		{
			if(signal == NULL) return;
			GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				RenderUnit* unit = units[i];
				MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
				if(munit != NULL)
				{
					Mesh* m = munit->getMesh();
					for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
					{
						SubMesh* submesh = m->getSubMeshList()[j];
						Material* mt = submesh->getMaterial();
						GpuProgram* gpu = mt->getGpuProgram();
						GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
						if(ggpu != NULL)
							ggpu->setUniformVariable(this->_type._name, *signal);
					}
				}
			}

		}
		virtual void retrieve(int* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileMeshPassFloatInputSocket : public FloatInputSocket
	{
	public:
		EtoileMeshPassFloatInputSocket(const std::string& name) : FloatInputSocket(name)
		{
		}

		virtual void perform(float* signal) override
		{
			if(signal == NULL) return;
			GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				RenderUnit* unit = units[i];
				MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
				if(munit != NULL)
				{
					Mesh* m = munit->getMesh();
					for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
					{
						SubMesh* submesh = m->getSubMeshList()[j];
						Material* mt = submesh->getMaterial();
						GpuProgram* gpu = mt->getGpuProgram();
						GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
						if(ggpu != NULL)
							ggpu->setUniformVariable(this->_type._name, *signal);
					}
				}
			}

		}
		virtual void retrieve(float* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileMeshPassViewMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileMeshPassViewMatrix4fInputSocket(const std::string& name = "In_ViewMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				RenderUnit* unit = units[i];
				MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
				if(munit != NULL)
				{
					Mesh* m = munit->getMesh();
					for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
					{
						SubMesh* submesh = m->getSubMeshList()[j];
						Material* mt = submesh->getMaterial();
						GpuProgram* gpu = mt->getGpuProgram();
						GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
						if(ggpu != NULL)
							ggpu->setUniformVariable(this->_type._name, *signal);
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};


	class EtoileMeshPassProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileMeshPassProjectionMatrix4fInputSocket(const std::string& name = "In_ProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				RenderUnit* unit = units[i];
				MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
				if(munit != NULL)
				{
					Mesh* m = munit->getMesh();
					for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
					{
						SubMesh* submesh = m->getSubMeshList()[j];
						Material* mt = submesh->getMaterial();
						GpuProgram* gpu = mt->getGpuProgram();
						GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
						if(ggpu != NULL)
							ggpu->setUniformVariable(this->_type._name, *signal);
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileMeshPassViewProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileMeshPassViewProjectionMatrix4fInputSocket(const std::string& name = "In_ViewProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLMeshRenderPass* plugin = (GLMeshRenderPass*)(this->getNode());
			std::vector<RenderUnit*>& units = plugin->getRenderUnits();
			for(unsigned int i = 0; i < units.size(); ++i)
			{
				RenderUnit* unit = units[i];
				MeshRenderUnit* munit = dynamic_cast<MeshRenderUnit*>(unit);
				if(munit != NULL)
				{
					Mesh* m = munit->getMesh();
					for(unsigned int j = 0; j < m->getSubMeshList().size(); ++j)
					{
						SubMesh* submesh = m->getSubMeshList()[j];
						Material* mt = submesh->getMaterial();
						GpuProgram* gpu = mt->getGpuProgram();
						GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
						if(ggpu != NULL)
							ggpu->setUniformVariable(this->_type._name, *signal);
					}
				}
			}

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileGLMeshRenderPassPlugin : public EPlugin, public GLMeshRenderPass 
	{
	public:
		EtoileGLMeshRenderPassPlugin(const std::string& name);
		~EtoileGLMeshRenderPassPlugin();
		bool loadFile(const std::string& filename);
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
	protected:
		ParameterInputSocketsWidget* widget;
		EtoileMeshInputSocket* _pinputmesh;
		EtoileMeshRenderUnitInputSocket* _pinputmeshunit;
		EtoileMeshPassViewMatrix4fInputSocket* _pview;
		EtoileMeshPassProjectionMatrix4fInputSocket* _pproj;
		EtoileMeshPassViewProjectionMatrix4fInputSocket * _pviewproj;
	};
}

