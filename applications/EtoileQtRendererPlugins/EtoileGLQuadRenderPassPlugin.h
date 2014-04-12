/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileGLQuadRenderPassPlugin.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "module/EPlugin.h"
#include "module/flowgraph/TextureSocket.h"
#include "renderer/OpenGL/GLQuadRenderPass.h"
#include "renderer/OpenGL/GLSLGpuProgram.h"
#include "ParameterInputSocketsWidget.h"
#include "module/flowgraph/GeneralSocket.h"

Etoile::EPlugin* loadEtoileGLQuadRenderPassPlugin();

namespace Etoile
{

	class EtoileQuadPassTextureInputSocket : public TextureInputSocket
	{
	public:
		EtoileQuadPassTextureInputSocket(const std::string& name);
		virtual void perform(Texture* signal) override;
		virtual void retrieve(Texture* signal) override;
	};


	class EtoileQuadPassIntInputSocket : public IntInputSocket
	{
	public:
		EtoileQuadPassIntInputSocket(const std::string& name):IntInputSocket(name){}
		virtual void perform(int* signal) override
		{
			if(signal == NULL) return;
			GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
			Material* mt = plugin->getMaterial();
			GpuProgram* gpu = mt->getGpuProgram();
			GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
			if(ggpu != NULL)
				ggpu->setUniformVariable(this->_type._name, *signal);
		}
		virtual void retrieve(int* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileQuadPassFloatInputSocket : public FloatInputSocket
	{
	public:
		EtoileQuadPassFloatInputSocket(const std::string& name) : FloatInputSocket(name)
		{
		}
		virtual void perform(float* signal) override
		{
			if(signal == NULL) return;
			GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
			Material* mt = plugin->getMaterial();
			GpuProgram* gpu = mt->getGpuProgram();
			GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
			if(ggpu != NULL)
				ggpu->setUniformVariable(this->_type._name, *signal);
		}
		virtual void retrieve(float* signal) override
		{
			if(signal == NULL) return;
		}
	};


	class EtoileQuadPassViewMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileQuadPassViewMatrix4fInputSocket(const std::string& name = "In_ViewMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
			Material* mt = plugin->getMaterial();
			GpuProgram* gpu = mt->getGpuProgram();
			GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
			if(ggpu != NULL)
				ggpu->setUniformVariable(this->_type._name, *signal);
		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};


	class EtoileQuadPassProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileQuadPassProjectionMatrix4fInputSocket(const std::string& name = "In_ProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
			Material* mt = plugin->getMaterial();
			GpuProgram* gpu = mt->getGpuProgram();
			GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
			if(ggpu != NULL)
				ggpu->setUniformVariable(this->_type._name, *signal);

		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};

	class EtoileQuadPassViewProjectionMatrix4fInputSocket : public Matrix4fInputSocket
	{
	public:
		EtoileQuadPassViewProjectionMatrix4fInputSocket(const std::string& name = "In_ViewProjectionMatrix") : Matrix4fInputSocket(name)
		{
		}
		virtual void perform(Matrix4f* signal) override
		{
			if(signal == NULL) return;
			GLQuadRenderPass* plugin = (GLQuadRenderPass*)(this->getNode());
			Material* mt = plugin->getMaterial();
			GpuProgram* gpu = mt->getGpuProgram();
			GLSLGpuProgram* ggpu = dynamic_cast<GLSLGpuProgram*>(gpu);
			if(ggpu != NULL)
				ggpu->setUniformVariable(this->_type._name, *signal);
		}
		virtual void retrieve(Matrix4f* signal) override
		{
			if(signal == NULL) return;
		}
	};
	

	class EtoileGLQuadRenderPassPlugin : public EPlugin, public GLQuadRenderPass 
	{
	public:
		EtoileGLQuadRenderPassPlugin(const std::string& name);
		~EtoileGLQuadRenderPassPlugin();
		bool loadFile(const std::string& filename);
		virtual void init() override;
		virtual void apply() override;
		virtual void release() override;
		void initMatrixSocket();
	protected:
		ParameterInputSocketsWidget* widget;
		EtoileQuadPassViewMatrix4fInputSocket* _pview;
		EtoileQuadPassProjectionMatrix4fInputSocket* _pproj;
		EtoileQuadPassViewProjectionMatrix4fInputSocket* _pviewproj;
	};
}

