/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Image.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/Vec4.h"

namespace Etoile
{
	class Image
	{
	public:
		Image(): _width(0), _height(0)
		{

		}
		Image(int width, int height): _width(width), _height(height)
		{

		}
		Image(const std::string& fileName){ _fileName = fileName;}
		void setFileName(const std::string& fileName){ _fileName = fileName;}
		void setWidth(int width){ _width = width;}
		void setHeight(int height){ _height = height;}
		int getWidth()const{ return _width;}
		int getHeight()const{ return _height;}
		const std::vector<Vec4f>& getData() const{return _data;}
		std::vector<Vec4f>& getData() {return _data;}
		void setData(std::vector<Vec4f>& data)
		{
			_data = data;
		}
	protected:
		int _width, _height;
		std::vector<Vec4f> _data;
		std::string _fileName;
	};
}

