#pragma once
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureSignal.h
* @brief 
* @date 1/2/2011
*/
#include "ESignal.h"
#include "geometry/Texture.h"

namespace Etoile
{
	typedef ESignal<Texture> TextureSignal;
	typedef ESignalPerformer<Texture> TextureSignalPerformer;
	typedef ESignalEmitter<Texture> TextureSignalEmitter;
	/*class TextureSignal : public ESignal<Texture>
	{
	public:
		TextureSignal(void);
		virtual ~TextureSignal(void);
	};

	class TextureSignalPerformer : public ESignalPerformer<Texture>
	{
	public:
		TextureSignalPerformer(void){}
		virtual ~TextureSignalPerformer(void){}
	};

	class TextureSignalEmitter : public ESignalEmitter<Texture>
	{
	public:
		TextureSignalEmitter(void){}
		virtual ~TextureSignalEmitter(void){}
	};*/

}