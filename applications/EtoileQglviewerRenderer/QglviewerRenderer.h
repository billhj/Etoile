/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QglviewerRenderer.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include <QGLViewer/qglviewer.h>
#include "renderer/RenderPass.h"

namespace Etoile
{
	class QglviewerPlugin;
	class QglviewerRenderer : public QGLViewer
	{
	public:
		QglviewerRenderer(QglviewerPlugin* plugin = NULL);
		~QglviewerRenderer();
		void init();
		void draw();
		void setRenderPasses(std::vector<RenderPass*>);
		void setOutputTexture(Texture *);
		Texture * getOutputTexture(){return _pOutput;}
		std::vector<RenderPass*>& getRenderPasses(){return _renderPasses;}
		void updateMatrix();
	private:
		Texture * _pOutput;
		std::vector<RenderPass*> _renderPasses;
		QglviewerPlugin* _pParent;
	};
}

