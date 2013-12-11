/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Pose.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "math/Vec3.h"

namespace Etoile
{
	class Offset
	{
	public:
		Offset(int index, Vec3f xyz):_index(index), _xyz(xyz){}
		int _index;
		Vec3f _xyz;
	};

	typedef std::vector<Offset *> Offsets;

	class Pose
	{
	public:
		Pose(int index, const std::string& name ,const std::string& target): _index(index), _name(name)
		{

		}
	
		Pose();
		~Pose();
		void addOffset(int index, Vec3f xyz);
		const Offsets& getOffsets()const { return _offsets;}
		const std::string& getName() const{ return _name;}

	private:
		int _index;
		std::string _name;
		Offsets _offsets;
	};
}