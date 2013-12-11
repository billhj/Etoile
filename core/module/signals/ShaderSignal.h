#pragma once
/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ShaderSignal.h
* @brief 
* @date 1/2/2011
*/
#include "ESignal.h"
#include "renderer/OpenGL/ShaderObject.h"

namespace Etoile
{
	typedef ESignal<ShaderObject> ShaderSignal;
	typedef ESignalPerformer<ShaderObject> ShaderSignalPerformer;
	typedef ESignalEmitter<ShaderObject> ShaderSignalEmitter;
	/*class ShaderSignal : public ESignal<ShaderObject>
	{
	public:
		ShaderSignal(void);
		virtual ~ShaderSignal(void);
	};

	class ShaderSignalPerformer : public ESignalPerformer<ShaderObject>
	{
	public:
		ShaderSignalPerformer(void){}
		virtual ~ShaderSignalPerformer(void){}
	};

	class ShaderSignalEmitter : public ESignalEmitter<ShaderObject>
	{
	public:
		ShaderSignalEmitter(void){}
		virtual ~ShaderSignalEmitter(void){}
	};*/

}