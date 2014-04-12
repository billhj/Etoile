/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GlutRenderer.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "renderer/RenderPass.h"


namespace Etoile
{
	class GlutRenderer
	{
	public:
		GlutRenderer();
		~GlutRenderer();
		void initGlut();
		void init();
		void draw();
		void setRenderPasses(std::vector<RenderPass*>);
		void setOutputTexture(Texture *);
	private:
		Texture * _pOutput;
		std::vector<RenderPass*> _renderPasses;
		int _w, _h;
	};
}

