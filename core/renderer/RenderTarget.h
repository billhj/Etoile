/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderTarget.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "module/flowgraph/TextureSocket.h"
#include "geometry/Texture.h"
namespace Etoile
{
	
	class TextureRenderTarget : public TextureOutputSocket
	{
	public:
		TextureRenderTarget(const std::string& name):TextureOutputSocket(name)
		{
		}
	};
}
