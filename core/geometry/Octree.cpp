/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Octree.cpp
* @brief 
* @date 1/2/2011
*/


#include "Octree.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	/*
	*	Octree struct doing insert delete find, Traversal 
	**/
		template<class T>
		Octree<T>::Octree(AxisAlignedBoundingBox<T> aabb, int level = 0, int idx = 0, int maxlevel = -1, int maxNbOfElements = 16) : _aabb(aabb), _level(level), _idx(idx), _maxLevels(maxlevel), _maxNbOfElements(maxNbOfElements), _isLeaf(true)
		{
			memset(_children, NULL, sizeof(_children));
		}

		/*Octree(AxisAlignedBoundingBox aabb, int level, int idx) : _aabb(aabb), _maxLevels(-1), _isLeaf(true), _level(level), _idx(idx), _maxNbOfElements(16)
		{
			memset(_children, NULL, sizeof(_children));
		}*/

		template<class T>
		void Octree<T>::addPoint(Vec3<T>& point)
		{
			addPointIntoNode(this, point);
		}

		template<class T>
		static void Octree<T>::addPointIntoNode(Octree* node, Vec3<T>& point)
		{
			if(node->isLeaf())
			{
				if(node->getPoints().size() < node->getMaxNbOfElements() || (node->getLevel() >= node->getMaxLevels() || node->getMaxLevels() < 0))
				{
					node->getPoints().push_back(point);
				}else
				{
					splitNode(node);
					addPointIntoNode(node, point);
				}
			}else
			{
				for (int j= 0; j < 8; j++)
				{
					if(node->getChildren()[j]->getAABB().isInclude(point))
					{
						addPointIntoNode(node->getChildren()[j], point);
					}
				}
			}
		}

		template<class T>
		static void Octree<T>::splitNode(Octree* node)
		{
			if(node->isLeaf())
			{
				AxisAlignedBoundingBox aabb[8];
				T xMin = node->getAABB().minimum().x();
				T yMin = node->getAABB().minimum().y();
				T zMin = node->getAABB().minimum().y();
				T xC = node->getAABB().center().x();
				T yC = node->getAABB().center().y();
				T zC = node->getAABB().center().y();
				T xMax = node->getAABB().maximum().x();
				T yMax = node->getAABB().maximum().y();
				T zMax = node->getAABB().maximum().y();
				aabb[0].setInterval(Vec3<T>(xMin,yMin,zMin), Vec3<T>(xC,yC,zC));
				aabb[0].setInterval(Vec3<T>(xC,yMin,zMin), Vec3<T>(xMax,yC,zC));
				aabb[0].setInterval(Vec3<T>(xMin,yC,zMin), Vec3<T>(xC,yMax,zC));
				aabb[0].setInterval(Vec3<T>(xC,yC,zMin), Vec3<T>(xMax,yMax,zC));
				aabb[0].setInterval(Vec3<T>(xMin,yMin,zC), Vec3<T>(xC,yC,zMax));
				aabb[0].setInterval(Vec3<T>(xC,yMin,zC), Vec3<T>(xMax,yC,zMax));
				aabb[0].setInterval(Vec3<T>(xMin,yC,zC), Vec3<T>(xC,yMax,zMax));
				aabb[0].setInterval(Vec3<T>(xC,yC,zC), Vec3<T>(xMax,yMax,zMax));
				
				for (int i= 0; i < 8; i++)
				{
					node->getChildren()[i] = new Octree(aabb[i], node->getLevel() + 1, i, node->getMaxLevels(), node->getMaxNbOfElements());
				}

				for(int i = 0 ; i < node->getPoints().size(); ++i)
				{
					Vec3<T> p = node->getPoints()[i];
					for (int j= 0; j < 8; i++)
					{
						if(node->getChildren()[j]->getAABB().isInclude(p))
						{
							node->getChildren()[j]->getPoints().push_back(p);
						}
					}
				}
				node->getPoints().clear();
				node->_isLeaf = false;
			}
		} 

		template<class T>
		bool Octree<T>::isLeaf(){return _isLeaf;}

		template<class T>
		AxisAlignedBoundingBox<T> Octree<T>::getAABB(){return _aabb;}

		template<class T>
		int Octree<T>::getLevel(){return _level;}

		template<class T>
		int Octree<T>::getIdx(){return _idx;}

		template<class T>
		int Octree<T>::getMaxLevels(){return _maxLevels;}

		template<class T>
		int Octree<T>::getMaxNbOfElements(){return _maxNbOfElements;}

		template<class T>
		std::vector<Vec3<T>>& Octree<T>::getPoints(){return _points;}

		template<class T>
		Octree<T>** Octree<T>::getChildren(){return _children;}
	
}