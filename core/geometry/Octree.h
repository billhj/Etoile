/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Octree.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "AxisAlignedBoundingBox.h"

namespace Etoile
{
	/*
	*	Octree struct doing insert delete find, Traversal 
	**/
	template<class T>
	class Octree
	{
		AxisAlignedBoundingBox<T> _aabb;
		
		int _level;
		int _idx;
		int _maxLevels;	
		int _maxNbOfElements;
		bool _isLeaf;
		Octree* _children[8];
		std::vector<Vec3<T>> _points;
	public:
		Octree(AxisAlignedBoundingBox<T> aabb, int level = 0, int idx = 0, int maxlevel = -1, int maxNbOfElements = 16);

		/*Octree(AxisAlignedBoundingBox aabb, int level, int idx) : _aabb(aabb), _maxLevels(-1), _isLeaf(true), _level(level), _idx(idx), _maxNbOfElements(16)
		{
			memset(_children, NULL, sizeof(_children));
		}*/

		void addPoint(Vec3<T>& point);

		static void addPointIntoNode(Octree* node, Vec3<T>& point);

		static void splitNode(Octree* node);

		bool isLeaf();
		AxisAlignedBoundingBox<T> getAABB();
		int getLevel();
		int getIdx();
		int getMaxLevels();
		int getMaxNbOfElements();
		std::vector<Vec3<T>>& getPoints();
		Octree** getChildren();
	};

	typedef Octree<double> Octreed;
	typedef Octree<float> Octreef;
}