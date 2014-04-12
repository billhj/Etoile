#pragma once

/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RgbaSignal.h
* @brief 
* @date 1/2/2011
*/
#include "math/Vec4.h"
#include "ESignal.h"

namespace Etoile
{
	typedef ESignal<Rgbaf> RgbaSignal;
	typedef ESignalPerformer<Rgbaf> RgbaSignalPerformer;
	typedef ESignalEmitter<Rgbaf> RgbaSignalEmitter;
	/*class RgbaSignal : public ESignal<Rgbaf>
	{
	public:
		RgbaSignal(void);
		virtual ~RgbaSignal(void);
	};

	class RgbaSignalPerformer : public ESignalPerformer<Rgbaf>
	{
	public:
		RgbaSignalPerformer(void){}
		virtual ~RgbaSignalPerformer(void){}
	};

	class RgbaSignalEmitter : public ESignalEmitter<Rgbaf>
	{
	public:
		RgbaSignalEmitter(void){}
		virtual ~RgbaSignalEmitter(void){}
	};*/
}