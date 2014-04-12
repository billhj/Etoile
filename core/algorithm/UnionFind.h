/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file UnionFind.h
* @brief 
* @date 1/2/2011
*/

#pragma once

namespace Etoile
{
	/*
	*	UnionFind class
	*	
	**/

	class UnionFind
	{
	protected:
		int* _id;
		int _count;  // number of component
	public:
		UnionFind(int n)
		{
			_id = new int[n];
			for(int i = 0; i < n; ++i)
			{
				_id[i] = i;
			}
			_count = n;
		}
		~UnionFind()
		{
			delete[] _id;
		}

		virtual void unite(int p, int q) = 0;

		virtual int find(int p) = 0;

		virtual bool connected(int p, int q) = 0;

		int count()
		{
			return _count;
		}
	};


	class QuickFindUF : public UnionFind
	{
		// _id[i] = component identifier of i
	public:
		QuickFindUF(int n) : UnionFind(n)
		{
		}

		virtual void unite(int p, int q) override
		{
			if (connected(p, q)) return;
			int pid = _id[p];
			int size = sizeof(_id) / sizeof(_id[0]);
			for (int i = 0; i < size; i++)
			{
				if (_id[i] == pid) 
					_id[i] = _id[q]; 
			}
			_count--;
		}

		virtual int find(int p) override
		{
			return _id[p];
		}

		virtual bool connected(int p, int q) override
		{
			return _id[p] == _id[q];
		}
	};

	class QuickUnionUF : public UnionFind
	{
		// _id[i] = parent of i
	public:
		QuickUnionUF(int n) : UnionFind(n)
		{
		}

		virtual void unite(int p, int q) override
		{
			int i = find(p);
			int j = find(q);
			if (i == j) return;
			_id[i] = j; 
			_count--;
		}

		virtual int find(int p) override
		{
			while (p != _id[p])
			{
				p = _id[p];
			}
			return p;
		}

		virtual bool connected(int p, int q) override
		{
			return find(p) == find(q);
		}
	};

	class WeightedQuickUnionUF  : public UnionFind
	{
	protected:
		int* _sz;    // _sz[i] = number of objects in subtree rooted at i
	public:
		WeightedQuickUnionUF (int n) : UnionFind(n)
		{
			_sz = new int[n];
			for(int i = 0; i < n; ++i)
			{
				_sz[i] = 1;
			}
		}

		virtual void unite(int p, int q) override
		{
			int i = find(p);
			int j = find(q);
			if (i == j) return;

			// make smaller root point to larger one
			if(_sz[i] < _sz[j]) 
			{ 
				_id[i] = j; 
				_sz[j] += _sz[i]; 
			}
			else                 
			{ 
				_id[j] = i; 
				_sz[i] += _sz[j]; 
			}
			_count--;
		}

		virtual int find(int p) override
		{
			while (p != _id[p])
			{
				p = _id[p];
			}
			return p;
		}

		virtual bool connected(int p, int q) override
		{
			return find(p) == find(q);
		}
	};


	class WeightedQuickUnionUFWithPathCompression : public WeightedQuickUnionUF
	{
	public:
		WeightedQuickUnionUFWithPathCompression (int n) : WeightedQuickUnionUF(n)
		{
		}

		virtual void unite(int p, int q) override
		{
			int i = find(p);
			int j = find(q);
			if (i == j) return;

			// make smaller root point to larger one
			if(_sz[i] < _sz[j]) 
			{ 
				_id[i] = j; 
				_sz[j] += _sz[i]; 
				//root(i);
			}
			else                 
			{ 
				_id[j] = i; 
				_sz[i] += _sz[j]; 
				//root(j);
			}

			_count--;
		}
		
		virtual int find(int p) override
		{
			int root = p;
			while (root != _id[root])
				root = _id[root];
			while (p != root) 
			{
				int newp = _id[p];
				_id[p] = root;
				p = newp;
			}
			return root;
		}

		/*
		int root(int i)
		{
			while(i != _id[i])
			{
				_id[i] = _id[_id[i]];
				i = _id[i];
			}
			return i;
		}*/
	};
}