#pragma once

/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageSignal.h
* @brief 
* @date 1/2/2011
*/
#include "util/ImageLoader/Image.h"
#include "ESignal.h"

namespace Etoile
{
	typedef ESignal<Image> ImageSignal;
	typedef ESignalPerformer<Image> ImageSignalPerformer;
	typedef ESignalEmitter<Image> ImageSignalEmitter;
	/*class ImageSignal : public ESignal<Image>
	{
	public:
		ImageSignal(void);
		virtual ~ImageSignal(void);
	};

	class ImageSignalPerformer : public ESignalPerformer<Image>
	{
	public:
		ImageSignalPerformer(void){}
		virtual ~ImageSignalPerformer(void){}
	};

	class ImageSignalEmitter : public ESignalEmitter<Image>
	{
	public:
		ImageSignalEmitter(void){}
		virtual ~ImageSignalEmitter(void){}
	};*/
}