/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BinaryHeap.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	enum HeapType
	{
		Max_Root = 0,
		Min_Root
	};

	/*
	*	BinaryHeap struct doing insert delete find, sort: insert always put into next pos in the array then update the tree
	*	advantage for sorting takes no more memory, only do exchange between two elements,  binary tree structure, complexity  n log_2 n  (n iteraion, log_2 n for depth of recursive process)  
	**/
	template <class K>
	class BinaryHeap
	{
	protected:
		std::vector<K> _heap;
		HeapType _type;
	public:
		BinaryHeap()
		{
			_type = Min_Root;
		}

		BinaryHeap(HeapType type): _type(type)
		{
		}

		HeapType getHeapType(){return _type;}
		void setHeapType(HeapType type){ _type = type;}

		std::vector<K>& getHeap(){return _heap;}
		K& getBinaryHeapNodeByIndex(unsigned int index)
		{
			return _heap[index];
		}

		void insertHeapNode(K node) 
		{
			_heap.push_back(node);
			heapifyUp(_heap.size() - 1);
		}

		//delete object by index, then up down to reorder the heap
		void deleteHeapNodeByIndex(int index)
		{
			//heapifyDown(index);
			K tmp = _heap[index];
			_heap[index] = _heap[_heap.size() - 1];
			_heap[_heap.size() - 1] = tmp;
			_heap.pop_back();
			heapifyUp(index);
			heapifyDown(index);
		}

		void deleteHeapRoot()
		{
			K tmp = _heap[0];
			_heap[0] = _heap[_heap.size() - 1];
			_heap[_heap.size() - 1] = tmp;
			_heap.pop_back();

			if(_type == Min_Root)
			{
				minHeapify(0, _heap.size());
			}
			else
			{
				maxHeapify(0, _heap.size());
			}
		}

		/*
		* compare with parent, if parent is larger, then do swap   (Root is min by defaut)
		**/
		void heapifyUp(int index)
		{
			if(_type == Min_Root)
			{
				while( ( index > 0 ) && ( getParentNodeIndex(index) >= 0 ) && ( _heap[index] < _heap[getParentNodeIndex(index)] ) )
				{
					K tmp = _heap[getParentNodeIndex(index)];
					_heap[getParentNodeIndex(index)] = _heap[index];
					_heap[index] = tmp;
					index = getParentNodeIndex(index);
				}
			}
			else
			{
				while( ( index > 0 ) && ( getParentNodeIndex(index) >= 0 ) && ( _heap[getParentNodeIndex(index)] < _heap[index] ) )
				{
					K tmp = _heap[getParentNodeIndex(index)];
					_heap[getParentNodeIndex(index)] = _heap[index];
					_heap[index] = tmp;
					index = getParentNodeIndex(index);
				}
			}
		}

		/*
		* compare with children, take the min child, swap parent with the min child, recursively make the parent to the leaf Node, then can achieve delete (Root is min by defaut)
		**/
		void heapifyDown(int index)
		{     
			if(_type == Min_Root)
			{
				int child = getLeftNodeIndex(index);
				int childRight = getRightNodeIndex(index);
				if ( ( child > 0 ) && ( childRight > 0 ) && ( _heap[childRight] < _heap[child] ) )
				{
					child = childRight;
				}
				if ( child > 0 )
				{
					K tmp = _heap[index];
					_heap[index] = _heap[child];
					_heap[child] = tmp;
					heapifyDown(child);
				}
			}
			else
			{
				int child = getLeftNodeIndex(index);
				int childRight = getRightNodeIndex(index);
				if ( ( child > 0 ) && ( childRight > 0 ) && ( _heap[child] < _heap[childRight] ) )
				{
					child = childRight;
				}
				if ( child > 0 )
				{
					K tmp = _heap[index];
					_heap[index] = _heap[child];
					_heap[child] = tmp;
					heapifyDown(child);
				}
			}
		}

		/*
		*	make a MinHeap
		*	check for the children, if a smaller child is smaller than parent, then change the place between parent and the smaller child, then recursively do checking
		**/
		void minHeapify(int index, int heapSize)
		{    
			int min = index;
			int childLeft = getLeftNodeIndex(index);
			int childRight = getRightNodeIndex(index);
			if ( ( childLeft > 0 ) && ( childLeft < heapSize ) && ( _heap[childLeft] < _heap[min] ) )
			{
				min = childLeft;
			}

			if ( ( childRight > 0 ) && ( childRight < heapSize ) && ( _heap[childRight] < _heap[min] ) )
			{
				min = childRight;
			}

			if (min != index)
			{
				K tmp = _heap[index];
				_heap[index] = _heap[min];
				_heap[min] = tmp;
				minHeapify(min, heapSize);
			}
		}

		/*
		*	make a MaxHeap
		**/
		void maxHeapify(int index, int heapSize)
		{    
			int max = index;
			int childLeft = getLeftNodeIndex(index);
			int childRight = getRightNodeIndex(index);
			if ( ( childLeft > 0 ) && ( childLeft < heapSize )  && (  _heap[max] < _heap[childLeft] ))
			{
				max = childLeft;
			}

			if ( ( childRight > 0 ) && ( childRight < heapSize )  && (  _heap[max] < _heap[childRight] ) )
			{
				max = childRight;
			}

			if (max != index)
			{
				K tmp = _heap[index];
				_heap[index] = _heap[max];
				_heap[max] = tmp;
				maxHeapify(max, heapSize);
			}
		}

		/*
		*	buildMinHeap root is min
		**/
		void buildMinHeap()
		{
			for(int i = (_heap.size() - 1)/2; i >= 0; i--)
			{
				minHeapify(i, _heap.size());
			}
			_type = Min_Root;
		}

		/*
		*	buildMaxHeap root is max
		**/
		void buildMaxHeap()
		{
			for(int i = (_heap.size() - 1)/2; i >= 0; i--)
			{
				maxHeapify(i, _heap.size());
			}
			_type = Max_Root;
		}

		/*
		*	by default (Root is min) generate a small  -> big MinHeap
		*	build MaxHeap, then change and put the max Root into the end of Heap, then use the rest (n - 1) as the new Heap rebuild MaxHeap, then reput the Root into the end of Heap
		*	iteratively put the max in the end.
		**/
		void heapSort()
		{
			if(_type == Min_Root)
			{
				buildMaxHeap();
				K temp;
				for(int i = _heap.size() - 1; i >= 0; i--)
				{
					temp = _heap[0];
					_heap[0] = _heap[i];
					_heap[i] = temp;
					maxHeapify(0, i);
				}
				_type = Min_Root;
			}
			else
			{
				buildMinHeap();
				K temp;
				for(int i = _heap.size() - 1; i >= 0; i--)
				{
					temp = _heap[0];
					_heap[0] = _heap[i];
					_heap[i] = temp;
					minHeapify(0, i);
				}
				_type = Max_Root;
			}
		}

		int getLeftNodeIndex(int parent)
		{
			int i = ( parent << 1 ) + 1; // 2 * parent + 1
			return ( i < (int)_heap.size() ) ? i : -1;
		}

		int getRightNodeIndex(int parent)
		{
			int i = ( parent << 1 ) + 2; // 2 * parent + 2
			return ( i < (int)_heap.size() ) ? i : -1;
		}

		int getParentNodeIndex(int child)
		{
			if (child != 0)
			{
				int i = (child - 1) >> 1;
				return i;
			}
			return -1;
		}


		bool empty()
		{
			return !(_heap.size() > 0);
		}
	};

	
	typedef BinaryHeap<double> BinaryHeapd;
}