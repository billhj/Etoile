/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Texture.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <map>

namespace Etoile
{
	class Texture
	{
	public:
		Texture(const std::string& name): _name(name), _width(0), _height(0), _layer(0)
		{
		}

		Texture(Texture& t)
		{
			_name = t._name;
			_width = t._width; _height = t._height;
			_layer = t._layer;
			_mipmaped = t._mipmaped;
		}
		virtual ~Texture(){}
		virtual void use() const = 0;
		virtual void unUse() const = 0;
		virtual void release() = 0;
		int getWidth() const
		{
			return _width;
		}
		int getHeight() const
		{
			return _height;
		}

		int getLayerNumbers() const
		{
			return _layer;
		}

		bool usingMipmap() const{ return _mipmaped;}
		virtual void generateMipmap()  const = 0;
		virtual void read(float* data) = 0;
		virtual void write(int x, int y, int width, int height, float* data) = 0;
		virtual void write(int size, float* data) = 0;
		const std::string getName(){return _name;}
		void setName(const std::string& name){ _name = name;}
		virtual void draw(int w, int h) = 0;
		virtual void draw() = 0;
	protected:
		int _width, _height, _layer;
		bool _mipmaped;
		std::string _name;
	};


}
