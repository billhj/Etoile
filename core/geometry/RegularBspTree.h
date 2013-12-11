/**
* Copyright(C) 2009-2012                
* @author Jing HUANG (original author: bert buchholz)
* @file BinaryHeap.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include <iostream>
#include <vector>
#include <cmath>

namespace Etoile
{
	// - Dimensions, size of each dimension
	// - Max depth or max amount of points per cell
	// - Enter a point into the tree
	// - When adding a point, check first if the max amount of points is
	//   excessed:
	//   - If not, add the point to the cell.
	//   - If yes, split the cell and redistribute the points into the new
	//     cells.

	template < class Point, size_t dim, class Data = int >
	class RegularBspTree
	{
	public:
		RegularBspTree(Point const& min, Point const& max, int maxDepth, int maxPoints) :
		  _cellMin(min),
			  _cellMax(max),
			  _isLeaf(true),
			  _depth(0),
			  _maxDepth(maxDepth),
			  _maxPoints(maxPoints),
			  _childrenCount(std::pow(2, dim))
		  { }

		  RegularBspTree() :
		  _isLeaf(true),
			  _depth(0),
			  _maxDepth(10),
			  _maxPoints(10),
			  _childrenCount(std::pow(2, dim))
		  { }

		  void getLeafs(std::vector< RegularBspTree<Point, dim>* >& leafs)
		  {
			  if (!_isLeaf)
			  {
				  for (int i = 0; i < _childrenCount; ++i)
				  {
					  _children[i].getLeafs(leafs);
				  }
			  }
			  else
			  {
				  leafs.push_back(this);
			  }
		  }

		  void setDepth(int d)
		  {
			  _depth = d;
		  }

		  bool getIsLeaf() const
		  {
			  return _isLeaf;
		  }

		  void setMin(Point const& min)
		  {
			  _cellMin = min;
		  }

		  void setMax(Point const& max)
		  {
			  _cellMax = max;
		  }

		  Point const& getMin() const
		  {
			  return _cellMin;
		  }

		  Point const& getMax() const
		  {
			  return _cellMax;
		  }

		  std::vector< RegularBspTree<Point, dim> > const& getChildren() const
		  {
			  return _children;
		  }

		  std::vector<Point> const& getPoints() const
		  {
			  return _points;
		  }

		  RegularBspTree<Point, dim> & addPoint(Point const& point, Data const& data = Data())
		  {
			  // std::cout << "Depth: " << _depth << " adding point: " << point << std::endl;

			  if (_isLeaf)
			  {
				  // add point into cell directly or if full, split the cell
				  // and add then
				  if ((int)_points.size() >= _maxPoints && _depth < _maxDepth)
				  {
					  // std::cout << "Full leaf, splitting." << std::endl;
					  split();

					  return addPoint(point, data);
				  }
				  else
				  {
					  // std::cout << "Leaf okay, pushing." << std::endl;
					  _points.push_back(point);
					  _data.push_back(data);

					  assert(_points.size() == _data.size());
				  }
			  }
			  else
			  {
				  // find the right child and add point there
				  // std::cout << "Not a leaf, adding to child." << std::endl;
				  for (int i = 0; i < _childrenCount; ++i)
				  {
					  if (_children[i].isPointIn(point))
					  {
						  return _children[i].addPoint(point, data);
					  }
				  }
			  }

			  return *this;
		  }


		  bool isPointIn(Point const& point) const
		  {
			  // return (point >= _cellMin && point <= _cellMax);

			  for (unsigned int i = 0; i < dim; ++i)
			  {
				  if (point[i] < _cellMin[i] || point[i] > _cellMax[i]) return false;
			  }

			  return true;
		  }

		  void split()
		  {
			  if (!_isLeaf) return;

			  _children.resize(_childrenCount, RegularBspTree<Point, dim>(_cellMin, _cellMax, _maxDepth, _maxPoints));

			  _isLeaf = false;

			  _children[0].setDepth(_depth + 1);

			  for (unsigned int d = 1; d <= dim; ++d)
			  {
				  int spacing = _childrenCount / std::pow(2, d);

				  for (unsigned int j = 0; j < std::pow(2, d - 1); ++j)
				  {
					  int parentIndex = j * spacing * 2;
					  int childIndex = parentIndex + spacing;
					  RegularBspTree const& parent = _children[parentIndex];

					  RegularBspTree leftChild = parent;
					  leftChild._children.clear();
					  RegularBspTree rightChild = parent;
					  rightChild._children.clear();

					  leftChild._cellMax[d - 1]  -= 0.5f * (parent._cellMax[d - 1] - parent._cellMin[d - 1]);
					  rightChild._cellMin[d - 1] += 0.5f * (parent._cellMax[d - 1] - parent._cellMin[d - 1]);

					  /*
					  std::cout <<
					  "d: " << d <<
					  " j: " << j <<
					  " spacing: " << spacing <<
					  " from: " << parentIndex <<
					  " to: " << parentIndex << ", " << childIndex << std::endl;
					  */

					  _children[parentIndex] = leftChild;
					  _children[childIndex] = rightChild;
				  }
			  }

			  for (unsigned int i = 0; i < _points.size(); ++i)
			  {
				  Point const& point = _points[i];
				  Data const& data = _data[i];

				  for (int j = 0; j < _childrenCount; ++j)
				  {
					  if (_children[j].isPointIn(point))
					  {
						  _children[j].addPoint(point, data);
						  break;
					  }
				  }
			  }

			  _points.clear();
			  _data.clear();
		  }

		  friend std::ostream& operator<< (std::ostream& stream, RegularBspTree const& q)
		  {
			  stream << q._depth << " " << q._cellMin << " " << q._cellMax << std::endl;

			  for (unsigned int i = 0; i < q._points.size(); ++i)
			  {
				  std::cout << "[" << q._points[i] << "] Data: " << q._data[i] << std::endl;
			  }


			  if (!q._isLeaf)
			  {
				  std::cout << "Children:" << std::endl;

				  for (int i = 0; i < q._childrenCount; ++i)
				  {
					  stream << q._children[i] << std::endl;
				  }
			  }

			  return stream;
		  }

		  RegularBspTree const& query(Point const& point) const
		  {
			  if (!_isLeaf)
			  {
				  for (int j = 0; j < _childrenCount; ++j)
				  {
					  if (_children[j].isPointIn(point))
					  {
						  return _children[j].query(point);
					  }
				  }
			  }

			  return *this;
		  }

	private:
		Point _cellMin;
		Point _cellMax;
		std::vector< RegularBspTree<Point, dim> > _children;

		std::vector<Point> _points;
		std::vector<Data> _data;

		bool _isLeaf;
		int _depth;
		int _maxDepth;
		int _maxPoints;

		int _childrenCount;
	};
}

//template <class Point>
//void draw(QPainter & p, RegularBspTree<Point, 2> const& quadTree, Point const& min, Point const& max)
//{
//    // std::cout << "Drawing" << std::endl;
//
//    p.setPen(Qt::black);
//
//    float xRange = max[0] - min[0];
//    float yRange = max[1] - min[1];
//
//    QRect rect(
//                std::floor((quadTree.getMin()[0] - min[0]) / xRange * p.window().width() + 0.5f),
//                std::floor((quadTree.getMin()[1] - min[1]) / yRange * p.window().height() + 0.5f),
//                std::floor((quadTree.getMax()[0] - quadTree.getMin()[0]) / xRange * p.window().width() + 0.5f),
//                std::floor((quadTree.getMax()[1] - quadTree.getMin()[1]) / yRange * p.window().width() + 0.5f)
//                );
//
//    p.drawRect(rect);
//
//    std::vector<Point> const& points = quadTree.getPoints();
//
//    p.setPen(Qt::red);
//
//    for (unsigned int i = 0; i < points.size(); ++i)
//    {
//        p.drawEllipse(
//                    (points[i][0] - min[0]) / xRange * p.window().width(),
//                    (points[i][1] - min[1]) / xRange * p.window().width(),
//                    2, 2);
//    }
//
//    if (!quadTree.getIsLeaf())
//    {
//        std::vector<RegularBspTree<Point, 2> > const& children = quadTree.getChildren();
//
//        for (unsigned int i = 0; i < children.size(); ++i)
//        {
//            draw(p, children[i], min, max);
//        }
//    }
//}
